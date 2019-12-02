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
   
  // Wenn eine neue Karte angelernt wird soll das Ende eines Tracks nicht
  // verarbeitet werden
  if (knownCard == false)
    return;

  switch(myCard.mode) {
  case MODE_RANDOM:
    Serial.println(F("Mode random -> stop"));
    break;

  case MODE_ALBUM:
    if (currentTrack != numTracksInFolder) {
      currentTrack = currentTrack + 1;
      mp3.playFolderTrack(myCard.folder, currentTrack);
      Serial.print(F("Albummodus active -> next track: "));
      Serial.print(currentTrack);
    }
    break;

  case MODE_PARTY:
    uint16_t oldTrack = currentTrack;
    currentTrack = newRandomTrack(numTracksInFolder);
    Serial.print(F("Party mode active -> play random track: "));
    Serial.println(currentTrack);
    mp3.playFolderTrack(myCard.folder, currentTrack);
    break;

  case MODE_SINGLE:
    Serial.println(F("Einzel mode active -> Strom sparen"));
    break;

  case MODE_BOOK:
    if (currentTrack != numTracksInFolder) {
      currentTrack = currentTrack + 1;
      Serial.println(F("Hörbuch mode active-> next track and save progress"));
      Serial.println(currentTrack);
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
    Serial.println(F("Mode random -> Play track again"));
    mp3.playFolderTrack(myCard.folder, currentTrack);
    break;

  case MODE_ALBUM:
    Serial.println(F("Album mode active -> vorheriger Track"));
    currentTrack = max(currentTrack - 1, 1);
    mp3.playFolderTrack(myCard.folder, currentTrack);
    break;

  case MODE_PARTY:
    Serial.println(F("Party mode active -> Track von vorne spielen"));
    mp3.playFolderTrack(myCard.folder, currentTrack);
    break;

  case MODE_SINGLE:
    Serial.println(F("Einzel mode active -> Track von vorne spielen"));
    mp3.playFolderTrack(myCard.folder, currentTrack);
    break;

  case MODE_BOOK:
    Serial.println(F("Hörbuch mode active-> previous track and save progress"));
    currentTrack = max(currentTrack - 1, 1);
    mp3.playFolderTrack(myCard.folder, currentTrack);
    saveProgress();
    break;
  }
}
