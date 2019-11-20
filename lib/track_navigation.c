static void nextTrack(uint16_t track) {
  if (track == _lastTrackFinished) {
    return;
   }
   _lastTrackFinished = track;
   
   if (knownCard == false)
    // Wenn eine neue Karte angelernt wird soll das Ende eines Tracks nicht
    // verarbeitet werden
    return;

  if (myCard.mode == 1) {
    Serial.println(F("Hörspielmodus ist aktiv -> keinen neuen Track spielen"));
//    mp3.sleep(); // Je nach Modul kommt es nicht mehr zurück aus dem Sleep!
  }
  if (myCard.mode == 2) {
    if (currentTrack != numTracksInFolder) {
      currentTrack = currentTrack + 1;
      mp3.playFolderTrack(myCard.folder, currentTrack);
      Serial.print(F("Albummodus ist aktiv -> nächster Track: "));
      Serial.print(currentTrack);
    } else 
//      mp3.sleep();   // Je nach Modul kommt es nicht mehr zurück aus dem Sleep!
    { }
  }
  if (myCard.mode == 3) {
    uint16_t oldTrack = currentTrack;
    currentTrack = random(1, numTracksInFolder + 1);
    if (currentTrack == oldTrack)
      currentTrack = currentTrack == numTracksInFolder ? 1 : currentTrack+1;
    Serial.print(F("Party Modus ist aktiv -> zufälligen Track spielen: "));
    Serial.println(currentTrack);
    mp3.playFolderTrack(myCard.folder, currentTrack);
  }
  if (myCard.mode == 4) {
    Serial.println(F("Einzel Modus aktiv -> Strom sparen"));
//    mp3.sleep();      // Je nach Modul kommt es nicht mehr zurück aus dem Sleep!
  }
  if (myCard.mode == 5) {
    if (currentTrack != numTracksInFolder) {
      currentTrack = currentTrack + 1;
      Serial.print(F("Hörbuch Modus ist aktiv -> nächster Track und "
                     "Fortschritt speichern"));
      Serial.println(currentTrack);
      mp3.playFolderTrack(myCard.folder, currentTrack);
      // Fortschritt im EEPROM abspeichern
      EEPROM.write(myCard.folder, currentTrack);
    } else {
//      mp3.sleep();  // Je nach Modul kommt es nicht mehr zurück aus dem Sleep!
      // Fortschritt zurück setzen
      EEPROM.write(myCard.folder, 1);
    }
  }
}

static void previousTrack() {
  if (myCard.mode == 1) {
    Serial.println(F("Hörspielmodus ist aktiv -> Track von vorne spielen"));
    mp3.playFolderTrack(myCard.folder, currentTrack);
  }
  if (myCard.mode == 2) {
    Serial.println(F("Albummodus ist aktiv -> vorheriger Track"));
    if (currentTrack != 1) {
      currentTrack = currentTrack - 1;
    }
    mp3.playFolderTrack(myCard.folder, currentTrack);
  }
  if (myCard.mode == 3) {
    Serial.println(F("Party Modus ist aktiv -> Track von vorne spielen"));
    mp3.playFolderTrack(myCard.folder, currentTrack);
  }
  if (myCard.mode == 4) {
    Serial.println(F("Einzel Modus aktiv -> Track von vorne spielen"));
    mp3.playFolderTrack(myCard.folder, currentTrack);
  }
  if (myCard.mode == 5) {
    Serial.println(F("Hörbuch Modus ist aktiv -> vorheriger Track und "
                     "Fortschritt speichern"));
    if (currentTrack != 1) {
      currentTrack = currentTrack - 1;
    }
    mp3.playFolderTrack(myCard.folder, currentTrack);
    // Fortschritt im EEPROM abspeichern
    EEPROM.write(myCard.folder, currentTrack);
  }
}

