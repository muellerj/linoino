bool isPlaying() { 
  return !digitalRead(busyPin); 
}

uint16_t newRandomTrack(uint16_t numTracksInFolder) {
  uint16_t newTrack = random(1, numTracksInFolder + 1);
  if (newTrack == currentTrack)
    newTrack = newTrack == numTracksInFolder ? 1 : newTrack + 1;
  return newTrack;
}

void saveProgress() {
  EEPROM.write(myCard.folder, currentTrack);
}

void resetProgress() {
  EEPROM.write(myCard.folder, 1);
}

void playAdvert() {
  mp3.playAdvertisement(currentTrack);
}

void pausePlayback() {
  mp3.pause();
}

void startPlayback() {
  mp3.start();
}

void nextTrack(uint16_t track) {
  if (track == lastTrackFinished)
    return;

  lastTrackFinished = track;
   
  // Return early if card is unknown
  if (knownCard == false)
    return;

  switch(myCard.mode) {
  case MODE_RANDOM:
    Serial.println(F("Mode random -> stop"));
    break;

  case MODE_SINGLE:
    Serial.println(F("Mode single -> stop"));
    break;

  case MODE_ALBUM:
    if (currentTrack < numTracksInFolder) {
      currentTrack = currentTrack + 1;
      printf("Mode album -> next track: %d\n", currentTrack);
      mp3.playFolderTrack(myCard.folder, currentTrack);
    }
    break;

  case MODE_PARTY:
    currentTrack = newRandomTrack(numTracksInFolder);
    printf("Mode party -> next random track: %d\n", currentTrack);
    mp3.playFolderTrack(myCard.folder, currentTrack);
    break;

  case MODE_BOOK:
    if (currentTrack < numTracksInFolder) {
      currentTrack = currentTrack + 1;
      printf("Mode book -> next track (saved): %d\n", currentTrack);
      mp3.playFolderTrack(myCard.folder, currentTrack);
      saveProgress();
    } else {
      resetProgress();
    }
    break;
  }
}

void previousTrack() {
  switch(myCard.mode) {
  case MODE_RANDOM:
    Serial.println(F("Mode random -> play track again"));
    break;

  case MODE_ALBUM:
    currentTrack = max(currentTrack - 1, 1);
    Serial.println(F("Mode album -> previous track"));
    break;

  case MODE_PARTY:
    Serial.println(F("Party mode -> Replay current track"));
    break;

  case MODE_SINGLE:
    Serial.println(F("Mode single-> Replay current track"));
    break;

  case MODE_BOOK:
    currentTrack = max(currentTrack - 1, 1);
    Serial.println(F("Mode book -> previous track (saved)"));
    saveProgress();
    break;
  }

  mp3.playFolderTrack(myCard.folder, currentTrack);
}
