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
  do {
    pauseButton.read();
    upButton.read();
    downButton.read();

    if (upButton.wasReleased() || downButton.wasReleased()) {
      Serial.print(F("Aborted!"));
      mp3.playMp3FolderTrack(802);
      return;
    }
  } while (!mfrc522.PICC_IsNewCardPresent());

  if (!mfrc522.PICC_ReadCardSerial())
    return;

  Serial.print(F("Reconfiguring card!"));
  setupCard();
}

void setupCard() {
  mp3.pause();
  Serial.print(F("Configure new card"));

  // Ordner abfragen
  myCard.folder = promptUserSelection(300, 1, 99, 0);

  // Wiedergabemodus abfragen
  myCard.mode = promptUserSelection(310, 311, 315, -1);

  // Hörbuchmodus -> Fortschritt im EEPROM auf 1 setzen
  resetProgress();

  // Einzelmodus -> Datei abfragen
  if (myCard.mode == MODE_SINGLE) {
    int folderCount = mp3.getFolderTrackCount(myCard.folder);
    myCard.special = promptUserSelection(320, 1, folderCount, myCard.folder);  
  }

  // Save settings
  mp3.pause();
  writeCard(myCard);
}

bool readCard(nfcTagObject *nfcTag) {
  bool returnValue = true;
  // Show some details of the PICC (that is: the tag/card)
  Serial.print(F("Card UID:"));
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  Serial.println();
  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));

  byte buffer[18];
  byte size = sizeof(buffer);

  // Authenticate using key A
  Serial.println(F("Authenticating using key A..."));
  status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(
      MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    returnValue = false;
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }

  // Show the whole sector as it currently is
  Serial.println(F("Current data in sector:"));
  mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
  Serial.println();

  // Read data from the block
  Serial.print(F("Reading data from block "));
  Serial.print(blockAddr);
  Serial.println(F(" ..."));
  status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    returnValue = false;
    Serial.print(F("MIFARE_Read() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
  }
  Serial.print(F("Data in block "));
  Serial.print(blockAddr);
  Serial.println(F(":"));
  dump_byte_array(buffer, 16);
  Serial.println();
  Serial.println();

  uint32_t tempCookie;
  tempCookie = (uint32_t)buffer[0] << 24;
  tempCookie += (uint32_t)buffer[1] << 16;
  tempCookie += (uint32_t)buffer[2] << 8;
  tempCookie += (uint32_t)buffer[3];

  nfcTag->cookie = tempCookie;
  nfcTag->version = buffer[4];
  nfcTag->folder = buffer[5];
  nfcTag->mode = buffer[6];
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
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    mp3.playMp3FolderTrack(401);
    return;
  }

  // Write data to the block
  Serial.print(F("Writing data into block "));
  Serial.print(blockAddr);
  Serial.println(F(" ..."));
  dump_byte_array(buffer, 16);
  Serial.println();
  status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(blockAddr, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
      mp3.playMp3FolderTrack(401);
  } else {
    mp3.playMp3FolderTrack(400);
  }
  Serial.println();
  delay(100);
}

void onNewCard() {
  randomSeed(millis() + random(1000));
  if (readCard(&myCard) == true) {
    if (myCard.cookie == 322417479 && myCard.folder != 0 && myCard.mode != 0) {
      knownCard = true;
      handleKnownCard();
    } else {
      knownCard = false;
      setupCard();
    }
  }
}

void handleKnownCard() {
  lastTrackFinished = 0;
  numTracksInFolder = mp3.getFolderTrackCount(myCard.folder);
  Serial.print(sprintf("%d files in folder:", numTracksInFolder));
  Serial.println(myCard.folder);

  switch(myCard.mode) {
    case MODE_RANDOM:
      Serial.println(F("Mode random -> Play random track"));
      currentTrack = random(1, numTracksInFolder + 1);
      break;

    case MODE_ALBUM:
      Serial.println(F("Mode album -> Play entire folder"));
      currentTrack = 1;
      break;

    case MODE_PARTY:
      Serial.println(F("Mode party -> Shuffle songs in folder"));
      currentTrack = random(1, numTracksInFolder + 1);
      break;

    case MODE_SINGLE:
      Serial.println(F("Mode single -> Play single file from folder"));
      currentTrack = myCard.special;
      break;

    case MODE_BOOK:
      Serial.println(F("Mode book -> Play entire folder and save progress"));
      currentTrack = EEPROM.read(myCard.folder);
      break;
  }

  mp3.playFolderTrack(myCard.folder, currentTrack);
}

byte pollCard() {
  if (!hasCard) {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() && readCard(&myCard)) {
      bool bSameUID = !memcmp(lastCardUid, mfrc522.uid.uidByte, 4);
      Serial.print(sprintf("Same card: %d", bSameUID ? "true" : "false"));
      // store info about current card
      memcpy(lastCardUid, mfrc522.uid.uidByte, 4);
      lastCardWasUL = mfrc522.PICC_GetType(mfrc522.uid.sak) == MFRC522::PICC_TYPE_MIFARE_UL;

      retries = 0;
      hasCard = true;
      return bSameUID ? PCS_CARD_IS_BACK : PCS_NEW_CARD;
    }
    return PCS_NO_CHANGE;
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
    }
  }
  return PCS_NO_CHANGE;
}

void handleCardReader() {
  uint8_t now = millis();

  if (static_cast<uint8_t>(now - lastCardPoll) > minPollInterval) {
    lastCardPoll = now;

    switch (pollCard()) {
      case PCS_NEW_CARD:
        onNewCard();
        break;

      case PCS_CARD_GONE:
        mp3.pause();
        setstandbyTimer();
        break;

      case PCS_CARD_IS_BACK:
        mp3.start();
        disablestandbyTimer();
        break;
    }
  }
}
