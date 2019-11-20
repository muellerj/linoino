bool isPlaying() { 
  return !digitalRead(busyPin); 
}

static void nextTrack(uint16_t track) {
  if (track == _lastTrackFinished) {
    return;
   }
   _lastTrackFinished = track;
   
   // Wenn eine neue Karte angelernt wird soll das Ende eines Tracks nicht
   // verarbeitet werden
   if (knownCard == false)
    return;

  switch(myCard.mode) {
  case MODE_HOERSPIEL:
    Serial.println(F("Hörspielmodus ist aktiv -> keinen neuen Track spielen"));
    break;

  case MODE_ALBUM:
    if (currentTrack != numTracksInFolder) {
      currentTrack = currentTrack + 1;
      mp3.playFolderTrack(myCard.folder, currentTrack);
      Serial.print(F("Albummodus ist aktiv -> nächster Track: "));
      Serial.print(currentTrack);
    }
    break;

  case MODE_PARTY:
    uint16_t oldTrack = currentTrack;
    currentTrack = random(1, numTracksInFolder + 1);
    if (currentTrack == oldTrack)
      currentTrack = currentTrack == numTracksInFolder ? 1 : currentTrack+1;
    Serial.print(F("Party Modus ist aktiv -> zufälligen Track spielen: "));
    Serial.println(currentTrack);
    mp3.playFolderTrack(myCard.folder, currentTrack);
    break;

  case MODE_SINGLE:
    Serial.println(F("Einzel Modus aktiv -> Strom sparen"));
    break;

  case MODE_BOOK:
    if (currentTrack != numTracksInFolder) {
      currentTrack = currentTrack + 1;
      Serial.print(F("Hörbuch Modus ist aktiv -> nächster Track und "
                     "Fortschritt speichern"));
      Serial.println(currentTrack);
      mp3.playFolderTrack(myCard.folder, currentTrack);
      // Fortschritt im EEPROM abspeichern
      EEPROM.write(myCard.folder, currentTrack);
    } else {
      // Fortschritt zurück setzen
      EEPROM.write(myCard.folder, 1);
    }
    break;
  }
}

static void previousTrack() {
  switch(myCard.mode) {
  case MODE_HOERSPIEL:
    Serial.println(F("Hörspielmodus ist aktiv -> Track von vorne spielen"));
    mp3.playFolderTrack(myCard.folder, currentTrack);
    break;

  case MODE_ALBUM:
    Serial.println(F("Albummodus ist aktiv -> vorheriger Track"));
    if (currentTrack != 1) {
      currentTrack = currentTrack - 1;
    }
    mp3.playFolderTrack(myCard.folder, currentTrack);
    break;

  case MODE_PARTY:
    Serial.println(F("Party Modus ist aktiv -> Track von vorne spielen"));
    mp3.playFolderTrack(myCard.folder, currentTrack);
    break;

  case MODE_SINGLE:
    Serial.println(F("Einzel Modus aktiv -> Track von vorne spielen"));
    mp3.playFolderTrack(myCard.folder, currentTrack);
    break;

  case MODE_BOOK:
    Serial.println(F("Hörbuch Modus ist aktiv -> vorheriger Track und "
                     "Fortschritt speichern"));
    if (currentTrack != 1) {
      currentTrack = currentTrack - 1;
    }
    mp3.playFolderTrack(myCard.folder, currentTrack);
    // Fortschritt im EEPROM abspeichern
    EEPROM.write(myCard.folder, currentTrack);
    break;
  }
}
