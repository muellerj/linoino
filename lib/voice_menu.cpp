void previewSelection(int folder, int track) {
  if (folder == -1) return;
  do { delay(10); } while (isPlaying());

  if (folder == 0) {
    folder = track;
    track = 1;
  }

  mp3.playFolderTrack(folder, track);
}

int changeOption(int i, int delta, int optMin, int optMax, int previewFolder) {
  i = delta > 0 ? min(i + delta, optMax - optMin + 1) : max(i + delta, 1);
  mp3.playMp3FolderTrack((optMin - 1) + i);
  delay(1000);
  previewSelection(previewFolder, i);
  return i;
}

void readButtons() {
  pauseButton.read();
  upButton.read();
  downButton.read();
}

int promptUserSelection(int question, int optMin, int optMax, 
                        int previewFolder=-1) {
  int returnValue = 0;
  mp3.playMp3FolderTrack(question);

  while(true) {

    readButtons();
    mp3.loop();

    // Confirm selection
    if (pauseButton.wasPressed() && returnValue != 0) return returnValue;

    // Up button
    if (upButton.pressedFor(LONG_PRESS)) {
      returnValue = changeOption(returnValue, +10, optMin, optMax, previewFolder);
      ignoreUpButton = true;
    } else if (upButton.wasReleased()) {
      if (!ignoreUpButton) {
      returnValue = changeOption(returnValue, +1, optMin, optMax, previewFolder);
      } else {
        ignoreUpButton = false;
      }
    }
    
    // Down button
    if (downButton.pressedFor(LONG_PRESS)) {
      returnValue = changeOption(returnValue, -10, optMin, optMax, previewFolder);
      ignoreDownButton = true;
    } else if (downButton.wasReleased()) {
      if (!ignoreDownButton) {
      returnValue = changeOption(returnValue, -1, optMin, optMax, previewFolder);
      } else {
        ignoreDownButton = false;
      }
    }
  }
}
