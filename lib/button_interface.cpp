void pollButtons() {

  pauseButton.read();
  upButton.read();
  downButton.read();

  // Pause button
  if (pauseButton.pressedFor(LONG_PRESS) && !ignorePauseButton) {
    if (isPlaying())
      mp3.playAdvertisement(currentTrack);
    else {
      knownCard = false;
      mp3.playMp3FolderTrack(800);
      Serial.println(F("Reset card..."));
      resetCard();
      mfrc522.PICC_HaltA();
      mfrc522.PCD_StopCrypto1();
    }
    ignorePauseButton = true;
  } else if (pauseButton.wasReleased()) {
    if (ignorePauseButton == false) {
      if (isPlaying())
        mp3.pause();
      else
        mp3.start();
    }
    ignorePauseButton = false;
  }

  // Up button
  if (upButton.pressedFor(LONG_PRESS)) {
    nextTrack(random(65536));
    ignoreUpButton = true;
  } else if (upButton.wasReleased()) {
    if (!ignoreUpButton) {
      Serial.println(F("Volume Up"));
      mp3.increaseVolume();
    } else {
      ignoreUpButton = false;
    }
  }

  // Down button
  if (downButton.pressedFor(LONG_PRESS)) {
    previousTrack();
    ignoreDownButton = true;
  } else if (downButton.wasReleased()) {
    if (!ignoreDownButton) {
      Serial.println(F("Volume Down"));
      mp3.decreaseVolume();
    } else {
      ignoreDownButton = false;
    }
  }
}

