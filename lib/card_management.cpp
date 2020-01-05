/*
 * Card types:
 *
 *   - SINGLE (play a single given file from one folder, then stop)
 *   - RANDOM (play a single random file from one folder, then stop)
 *   - ALBUM (play all files from one folder in order, stop at end)
 *   - BOOK  (play all files from one folder in order, save progress, stop at end)
 *   - PARTY (play all files from one folder randomly, never stop)
 *
 */

void resetCard() {
  Serial.println(F("Reset card..."));
  playMessage(800);

  do {
    switch(pollButtons()) {
      case BTN_UP_SHORTPRESS: 
      case BTN_DOWN_SHORTPRESS:
        Serial.println(F("Aborted!"));
        playMessage(802);
        return;
        break;
      case BTN_PAUSE_SHORTPRESS:
        Serial.println(F("Restarting..."));
        fullReset();
        break;
    }
  } while (!mfrc522.PICC_IsNewCardPresent());

  if (!mfrc522.PICC_ReadCardSerial())
    return;

  setupCard(true);
  mfrc522.PICC_HaltA();
  mfrc522.PCD_StopCrypto1();
}

void forgetCard() {
  memcpy(lastCardUid, 0, 4);
}

void rememberCard() {
  memcpy(lastCardUid, mfrc522.uid.uidByte, 4);
}

bool isSameCard() {
  return (memcmp(lastCardUid, mfrc522.uid.uidByte, 4) == 0);
}

void setupCard(bool reset = false) {
  pausePlayback();
  Serial.println(F("Configure new card"));

  myCard.folder = promptUserSelection(300, 1, 99, 0);
  myCard.mode   = promptUserSelection(310, 311, 315, -1);

  currentTrack = 1;
  saveProgress();

  if (myCard.mode == MODE_SINGLE) {
    myCard.special = promptUserSelection(320, 1, getTrackCount(), myCard.folder);  
  }

  writeCard(myCard);

  if (reset) {
    playMessage(999);
  } else {
    playMessage(998);
  }

  forgetCard();
}

bool readCard(nfcTagObject *nfcTag) {
  bool returnValue = true;
  // Show some details of the PICC (that is: the tag/card)
  Serial.println(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println(F(""));
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println("PICC type: " + String(mfrc522.PICC_GetTypeName(piccType)));

  byte buffer[18];
  byte size = sizeof(buffer);

  // Authenticate using key A
  Serial.println(F("Authenticating using key A..."));
  status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(
      MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    returnValue = false;
    Serial.println("PCD_Authenticate() failed: " + String(mfrc522.GetStatusCodeName(status)));
  }

  // Show the whole sector as it currently is
  Serial.println(F("Current data in sector:"));
  mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
  Serial.println(F(""));

  // Read data from the block
  Serial.println("Reading data from block " + String(blockAddr) + "...");
  status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    returnValue = false;
    Serial.println("MIFARE_Read() failed: " + String(mfrc522.GetStatusCodeName(status)));
  }

  // printf("Data in block %d:", blockAddr);
  // dump_byte_array(buffer, 16);
  // Serial.println(F(""));
  // Serial.println(F(""));

  uint32_t tempCookie;
  tempCookie = (uint32_t)buffer[0] << 24;
  tempCookie += (uint32_t)buffer[1] << 16;
  tempCookie += (uint32_t)buffer[2] << 8;
  tempCookie += (uint32_t)buffer[3];

  nfcTag->cookie  = tempCookie;
  nfcTag->version = buffer[4];
  nfcTag->folder  = buffer[5];
  nfcTag->mode    = buffer[6];
  nfcTag->special = buffer[7];

  return returnValue;
}

void writeCard(nfcTagObject nfcTag) {
  MFRC522::PICC_Type mifareType;
  byte buffer[16] = {0x13, 0x37, 0xb3, 0x47, // 0x1337 0xb347 magic cookie to
                                             // identify our nfc tags
                     0x01,                   // version 1
                     nfcTag.folder,          // the folder picked by the user
                     nfcTag.mode,    // the playback mode picked by the user
                     nfcTag.special, // track or function for admin cards
                     0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

  byte size = sizeof(buffer);

  mifareType = mfrc522.PICC_GetType(mfrc522.uid.sak);

  // Authenticate using key B
  Serial.println(F("Authenticating again using key B..."));
  status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(
      MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.println("PCD_Authenticate() failed: " + String(mfrc522.GetStatusCodeName(status)));
    playMessage(401);
    return;
  }

  // Write data to the block
  //printf("Writing data into block %d...\n", blockAddr);
  //dump_byte_array(buffer, 16);
  //Serial.println(F(""));

  status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(blockAddr, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.println("MIFARE_Write() failed: " + String(mfrc522.GetStatusCodeName(status)));
    playMessage(401);
  } else {
    playMessage(400);
  }
  Serial.println(F(""));
  delay(100);
}

void onNewCard() {
  randomSeed(millis() + random(1000));
  if (readCard(&myCard) == true) {
    if (myCard.cookie == 322417479 && myCard.folder != 0 && myCard.mode != 0) {
      handleKnownCard();
    } else {
      setupCard();
    }
  }
}

void handleKnownCard() {
  lastTrackFinished = 0;
  trackCount = getTrackCount();
  Serial.println(String(trackCount) + " files in folder " + String(myCard.folder));

  switch(myCard.mode) {
    case MODE_ALBUM:  currentTrack = 1; break;
    case MODE_RANDOM:
    case MODE_PARTY:  currentTrack = random(1, trackCount + 1); break;
    case MODE_SINGLE: currentTrack = myCard.special; break;
    case MODE_BOOK:   currentTrack = EEPROM.read(myCard.folder); break;
  }

  playTrack(currentTrack);
}

byte pollCard() {
  uint8_t now = millis();
  uint8_t timeGone = static_cast<uint8_t>(now - lastCardPoll);

  if (timeGone < minPollInterval)
    return PCS_NO_CHANGE;

  lastCardPoll = now;

  if (!hasCard) {
    if (mfrc522.PICC_IsNewCardPresent() && \
        mfrc522.PICC_ReadCardSerial() && \
        readCard(&myCard)) {
      bool sameCard = isSameCard();
      Serial.println("Same card: " + String(sameCard));
      rememberCard();
      lastCardWasUL = mfrc522.PICC_GetType(mfrc522.uid.sak) == MFRC522::PICC_TYPE_MIFARE_UL;

      retries = 0;
      hasCard = true;
      return sameCard ? PCS_CARD_IS_BACK : PCS_NEW_CARD;
    } else {
      return PCS_NO_CHANGE;
    }
  } else {
    // perform a dummy read command just to see whether the card is in range
    byte buffer[18];
    byte size = sizeof(buffer);

    if (mfrc522.MIFARE_Read(lastCardWasUL ? 8 : blockAddr, buffer, &size) != MFRC522::STATUS_OK) {
      if (retries < maxRetries) {
        retries++;
      } else {
        Serial.println(F("card gone"));
        mfrc522.PICC_HaltA();
        mfrc522.PCD_StopCrypto1();
        hasCard = false;
        return PCS_CARD_GONE;
      }
    } else {
      retries = 0;
      return PCS_NO_CHANGE;
    }
  }
}
