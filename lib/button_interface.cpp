void handleButtonPresses() {

  pauseButton.read();
  upButton.read();
  downButton.read();

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

  if (upButton.pressedFor(LONG_PRESS)) {
    Serial.println(F("Volume Up"));
    mp3.increaseVolume();
    ignoreUpButton = true;
  } else if (upButton.wasReleased()) {
    if (!ignoreUpButton)
      nextTrack(random(65536));
    else
      ignoreUpButton = false;
  }

  if (downButton.pressedFor(LONG_PRESS)) {
    Serial.println(F("Volume Down"));
    mp3.decreaseVolume();
    ignoreDownButton = true;
  } else if (downButton.wasReleased()) {
    if (!ignoreDownButton)
      previousTrack();
    else
      ignoreDownButton = false;
  }
}

