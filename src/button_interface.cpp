#include "headers.h"

Button pauseButton(buttonPause);
Button upButton(buttonUp);
Button downButton(buttonDown);

bool ignorePauseButton = false;
bool ignoreUpButton    = false;
bool ignoreDownButton  = false;

int pollButtons() {
  pauseButton.read();
  upButton.read();
  downButton.read();

  // Pause button
  if (pauseButton.pressedFor(LONG_PRESS) && !ignorePauseButton) {
    ignorePauseButton = true;
    return BTN_PAUSE_LONGPRESS;
  } else if (pauseButton.wasReleased()) {
    if (ignorePauseButton == false) {
      return BTN_PAUSE_SHORTPRESS;
    } else {
      ignorePauseButton = false;
    }
  }

  // Up button
  if (upButton.pressedFor(LONG_PRESS) && !ignoreUpButton) {
    ignoreUpButton = true;
    return BTN_UP_LONGPRESS;
  } else if (upButton.wasReleased()) {
    if (ignoreUpButton == false) {
      return BTN_UP_SHORTPRESS;
    } else {
      ignoreUpButton = false;
    }
  }

  // Down button
  if (downButton.pressedFor(LONG_PRESS) && !ignoreDownButton) {
    ignoreDownButton = true;
    return BTN_DOWN_LONGPRESS;
  } else if (downButton.wasReleased()) {
    if (ignoreDownButton == false) {
      return BTN_DOWN_SHORTPRESS;
    } else {
      ignoreDownButton = false;
    }
  }

  return BTN_NOTHING;
}
