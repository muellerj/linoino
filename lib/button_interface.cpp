void volumeUp() {
  Serial.println(F("Volume Up"));
  mp3.increaseVolume();
}

void volumeDown() {
  Serial.println(F("Volume Down"));
  mp3.decreaseVolume();
}

int pollButtons() {
  pauseButton.read();
  upButton.read();
  downButton.read();

  // Pause button
  if (pauseButton.pressedFor(LONG_PRESS) && !ignorePauseButton) {
    ignorePauseButton = true;
    Serial.println(F("Button PAUSE long press"));
    return BTN_PAUSE_LONGPRESS;
  } else if (pauseButton.wasReleased()) {
    if (ignorePauseButton == false) {
      Serial.println(F("Button PAUSE short press"));
      return BTN_PAUSE_SHORTPRESS;
    } else {
      ignorePauseButton = false;
    }
  }

  // Up button
  if (upButton.pressedFor(LONG_PRESS) && !ignoreUpButton) {
    ignoreUpButton = true;
    Serial.println(F("Button UP long press"));
    return BTN_UP_LONGPRESS;
  } else if (upButton.wasReleased()) {
    if (ignoreUpButton == false) {
      Serial.println(F("Button UP short press"));
      return BTN_UP_SHORTPRESS;
    } else {
      ignoreUpButton = false;
    }
  }

  // Down button
  if (downButton.pressedFor(LONG_PRESS) && !ignoreDownButton) {
    ignoreDownButton = true;
    Serial.println(F("Button DOWN long press"));
    return BTN_DOWN_LONGPRESS;
  } else if (downButton.wasReleased()) {
    if (ignoreDownButton == false) {
      Serial.println(F("Button DOWN short press"));
      return BTN_DOWN_SHORTPRESS;
    } else {
      ignoreDownButton = false;
    }
  }

  return BTN_NOTHING;
}
