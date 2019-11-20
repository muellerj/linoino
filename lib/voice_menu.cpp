void previewCurrentSelection(int previewFromFolder, int returnValue) {
  do {
    delay(10);
  } while (isPlaying());
  if (previewFromFolder == 0)
    mp3.playFolderTrack(returnValue, 1);
  else
    mp3.playFolderTrack(previewFromFolder, returnValue);
}

int voiceMenu(int numberOfOptions, int startMessage, int messageOffset,
              bool preview = false, int previewFromFolder = 0) {

  int returnValue = 0;

  if (startMessage != 0) {
    mp3.playMp3FolderTrack(startMessage);
  }

  while(true) {

    pauseButton.read();
    upButton.read();
    downButton.read();

    mp3.loop();

    // Pause button
    if (pauseButton.wasPressed()) {
      if (returnValue != 0) return returnValue;
      delay(1000);
    }

    // Up button
    if (upButton.pressedFor(LONG_PRESS)) {
      returnValue = min(returnValue + 10, numberOfOptions);
      mp3.playMp3FolderTrack(messageOffset + returnValue);
      delay(1000);
      if (preview) {
        previewCurrentSelection(previewFromFolder, returnValue);
      }
      ignoreUpButton = true;
    } else if (upButton.wasReleased()) {
      if (!ignoreUpButton) {
        returnValue = min(returnValue + 1, numberOfOptions);
        mp3.playMp3FolderTrack(messageOffset + returnValue);
        delay(1000);
        if (preview) {
          previewCurrentSelection(previewFromFolder, returnValue);
        }
      } else {
        ignoreUpButton = false;
      }
    }
    
    // Down button
    if (downButton.pressedFor(LONG_PRESS)) {
      returnValue = max(returnValue - 10, 1);
      mp3.playMp3FolderTrack(messageOffset + returnValue);
      delay(1000);
      if (preview) {
        previewCurrentSelection(previewFromFolder, returnValue);
      }
      ignoreDownButton = true;
    } else if (downButton.wasReleased()) {
      if (!ignoreDownButton) {
        returnValue = max(returnValue - 1, 1);
        mp3.playMp3FolderTrack(messageOffset + returnValue);
        delay(1000);
        if (preview) {
          previewCurrentSelection(previewFromFolder, returnValue);
        }
      } else {
        ignoreDownButton = false;
      }
    }
  }
}

