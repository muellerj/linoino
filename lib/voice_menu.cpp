void previewSelection(int previewFromFolder, int returnValue) {
  do {
    delay(10);
  } while (isPlaying());
  if (previewFromFolder == 0)
    mp3.playFolderTrack(returnValue, 1);
  else
    mp3.playFolderTrack(previewFromFolder, returnValue);
}

int changeOption(int returnValue, int delta, int numberOfOptions, 
                 int messageOffset, bool preview, int previewFromFolder) {
  if (delta > 0) {
    returnValue = min(returnValue + delta, numberOfOptions);
  } else {
    returnValue = max(returnValue + delta, 1);
  }
  mp3.playMp3FolderTrack(messageOffset + returnValue);
  delay(1000);
  if (preview)
    previewSelection(previewFromFolder, returnValue);
  return returnValue;
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
      returnValue = changeOption(returnValue, 10, numberOfOptions, 
                                 messageOffset, preview, previewFromFolder);
      ignoreUpButton = true;
    } else if (upButton.wasReleased()) {
      if (!ignoreUpButton) {
        returnValue = changeOption(returnValue, 1, numberOfOptions, 
                                   messageOffset, preview, previewFromFolder);
      } else {
        ignoreUpButton = false;
      }
    }
    
    // Down button
    if (downButton.pressedFor(LONG_PRESS)) {
      returnValue = changeOption(returnValue, -10, numberOfOptions, 
                                 messageOffset, preview, previewFromFolder);
      ignoreDownButton = true;
    } else if (downButton.wasReleased()) {
      if (!ignoreDownButton) {
        returnValue = changeOption(returnValue, -1, numberOfOptions, 
                                   messageOffset, preview, previewFromFolder);
      } else {
        ignoreDownButton = false;
      }
    }
  }
}
