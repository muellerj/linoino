#include "headers.h"

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(A0));
  pinMode(busyPin, INPUT);

  Serial.println(F("LinUINO Version 1.0"));
  Serial.println(F("based on work by Thorsten Voß"));

  setupButtons();
  setupCardReader();
  setupMp3Player();
  setstandbyTimer();
}

void loop() {
  mp3.loop();

  if (isPlaying()) disablestandbyTimer();
  
  switch(pollStandby()) {
    case STDBY_DISABLED:
    case STDBY_NOT_YET:  noOp(); break;
    case STDBY_ACTIVATE: setStandby(); break;
  }

  switch(pollSleep()) {
    case SLEEP_DISABLED:
    case SLEEP_NOT_YET:  noOp(); break;
    case SLEEP_ACTIVATE: goToSleep(); break;
  }

  switch(pollButtons()) {
    case BTN_PAUSE_LONGPRESS:  isPlaying() ? toggleSleepTimer() : resetCard(); break;
    case BTN_PAUSE_SHORTPRESS: hasCard ? startPlayback() : noOp(); break;
    case BTN_UP_LONGPRESS:     nextTrack(random(65536)); break;
    case BTN_UP_SHORTPRESS:    volumeUp(); break;
    case BTN_DOWN_LONGPRESS:   previousTrack(); break;
    case BTN_DOWN_SHORTPRESS:  volumeDown(); break;
  }

  switch (pollCard()) {
    case PCS_NEW_CARD:     onNewCard(); break;
    case PCS_CARD_GONE:    pausePlayback(); break;
    case PCS_CARD_IS_BACK: startPlayback(); break;
  }
}
