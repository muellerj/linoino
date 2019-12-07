void volumeUp() {
  printf("Volume Up\n");
  mp3.increaseVolume();
}

void volumeDown() {
  printf("Volume Down\n");
  mp3.decreaseVolume();
}

int pollButtons() {
  pauseButton.read();
  upButton.read();
  downButton.read();

  // Pause button
  if (pauseButton.pressedFor(LONG_PRESS) && !ignorePauseButton) {
    ignorePauseButton = true;
    printf("Button PAUSE long press\n");
    return BTN_PAUSE_LONGPRESS;
  } else if (pauseButton.wasReleased()) {
    if (ignorePauseButton == false) {
      printf("Button PAUSE short press\n");
      return BTN_PAUSE_SHORTPRESS;
    } else {
      ignorePauseButton = false;
    }
  }

  // Up button
  if (upButton.pressedFor(LONG_PRESS) && !ignoreUpButton) {
    ignoreUpButton = true;
    printf("Button UP long press\n");
    return BTN_UP_LONGPRESS;
  } else if (upButton.wasReleased()) {
    if (ignoreUpButton == false) {
      printf("Button UP short press\n");
      return BTN_UP_SHORTPRESS;
    } else {
      ignoreUpButton = false;
    }
  }

  // Down button
  if (downButton.pressedFor(LONG_PRESS) && !ignoreDownButton) {
    ignoreDownButton = true;
    printf("Button DOWN long press\n");
    return BTN_DOWN_LONGPRESS;
  } else if (downButton.wasReleased()) {
    if (ignoreDownButton == false) {
      printf("Button DOWN short press\n");
      return BTN_DOWN_SHORTPRESS;
    } else {
      ignoreDownButton = false;
    }
  }

  return BTN_NOTHING;
}
