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
  printf("Reset card...\n");
  knownCard = false;
  playMessage(800);

  do {
    switch(pollButtons()) {
      case BTN_UP_SHORTPRESS: 
      case BTN_DOWN_SHORTPRESS:
        printf("Aborted!\n");
        playMessage(802);
        return;
      break;
    }
  } while (!mfrc522.PICC_IsNewCardPresent());

  if (!mfrc522.PICC_ReadCardSerial())
    return;

  setupCard();
  playMessage(999);
  memcpy(lastCardUid, 0, 4);
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
  return !memcmp(lastCardUid, mfrc522.uid.uidByte, 4);
}

void setupCard() {
  pausePlayback();
  printf("Configure new card\n");

  myCard.folder = promptUserSelection(300, 1, 99, 0);
  myCard.mode   = promptUserSelection(310, 311, 315, -1);

  currentTrack = 1;
  saveProgress();

  if (myCard.mode == MODE_SINGLE) {
    myCard.special = promptUserSelection(320, 1, getTrackCount(), myCard.folder);  
  }

  pausePlayback();
  writeCard(myCard);
}

bool readCard(nfcTagObject *nfcTag) {
  bool returnValue = true;
  // Show some details of the PICC (that is: the tag/card)
  printf("Card UID:\n");
  dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
  printf("\n");
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  printf("PICC type: %d\n", mfrc522.PICC_GetTypeName(piccType));

  byte buffer[18];
  byte size = sizeof(buffer);

  // Authenticate using key A
  printf("Authenticating using key A...\n");
  status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(
      MFRC522::PICC_CMD_MF_AUTH_KEY_A, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    returnValue = false;
    printf("PCD_Authenticate() failed: %d\n", mfrc522.GetStatusCodeName(status));
    return;
  }

  // Show the whole sector as it currently is
  printf("Current data in sector:\n");
  mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, sector);
  printf("\n");

  // Read data from the block
  printf("Reading data from block %d...\n", blockAddr);
  status = (MFRC522::StatusCode)mfrc522.MIFARE_Read(blockAddr, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    returnValue = false;
    printf("MIFARE_Read() failed: %d\n", mfrc522.GetStatusCodeName(status));
  }

  // printf("Data in block %d:", blockAddr);
  // dump_byte_array(buffer, 16);
  // printf("\n");
  // printf("\n");

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
  printf("Authenticating again using key B...\n");
  status = (MFRC522::StatusCode)mfrc522.PCD_Authenticate(
      MFRC522::PICC_CMD_MF_AUTH_KEY_B, trailerBlock, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) {
    printf("PCD_Authenticate() failed: %d\n", mfrc522.GetStatusCodeName(status));
    playMessage(401);
    return;
  }

  // Write data to the block
  //printf("Writing data into block %d...\n", blockAddr);
  //dump_byte_array(buffer, 16);
  //printf("\n");

  status = (MFRC522::StatusCode)mfrc522.MIFARE_Write(blockAddr, buffer, 16);
  if (status != MFRC522::STATUS_OK) {
    printf("MIFARE_Write() failed: %d\n", mfrc522.GetStatusCodeName(status));
    playMessage(401);
  } else {
    playMessage(400);
  }
  printf("\n");
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
  trackCount = getTrackCount();
  printf("%d files in folder %d\n", trackCount, myCard.folder);

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
  if (!hasCard) {
    if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial() && readCard(&myCard)) {
      bool sameCard = isSameCard();
      printf("Same card: %s\n", sameCard ? "true" : "false");
      rememberCard();
      lastCardWasUL = mfrc522.PICC_GetType(mfrc522.uid.sak) == MFRC522::PICC_TYPE_MIFARE_UL;

      retries = 0;
      hasCard = true;
      return sameCard ? PCS_CARD_IS_BACK : PCS_NEW_CARD;
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
          printf("card gone\n");
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
