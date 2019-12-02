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

void nextTrack(uint16_t track) {
  if (track == _lastTrackFinished)
    return;

  _lastTrackFinished = track;
   
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
      Serial.print(sprintf("Mode album -> next track: %d", currentTrack));
      mp3.playFolderTrack(myCard.folder, currentTrack);
    }
    break;

  case MODE_PARTY:
    currentTrack = newRandomTrack(numTracksInFolder);
    Serial.print(sprintf("Mode party -> next random track: %d", currentTrack));
    mp3.playFolderTrack(myCard.folder, currentTrack);
    break;

  case MODE_BOOK:
    if (currentTrack < numTracksInFolder) {
      currentTrack = currentTrack + 1;
      Serial.print(sprintf("Mode book -> next track (saved): %d", currentTrack));
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
    mp3.playFolderTrack(myCard.folder, currentTrack);
    break;

  case MODE_ALBUM:
    currentTrack = max(currentTrack - 1, 1);
    Serial.println(F("Mode album -> previous track"));
    mp3.playFolderTrack(myCard.folder, currentTrack);
    break;

  case MODE_PARTY:
    Serial.println(F("Party mode -> Replay current track"));
    mp3.playFolderTrack(myCard.folder, currentTrack);
    break;

  case MODE_SINGLE:
    Serial.println(F("Mode single-> Replay current track"));
    mp3.playFolderTrack(myCard.folder, currentTrack);
    break;

  case MODE_BOOK:
    currentTrack = max(currentTrack - 1, 1);
    Serial.println(F("Mode book -> previous track (saved)"));
    mp3.playFolderTrack(myCard.folder, currentTrack);
    saveProgress();
    break;
  }
}
