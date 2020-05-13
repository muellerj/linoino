#include "headers.h"

unsigned long sleepTimer = 15UL * 60UL * 1000UL; // miliseconds
unsigned long sleepMillis = 0; // miliseconds

void setSleepTimer() {
  Serial.println("Sleep in " + String(sleepTimer / 1000UL) + "s");
  sleepMillis = millis() + sleepTimer;
}

void disableSleepTimer() {
  if (sleepMillis != 0) {
    Serial.println(F("Disabling sleep timer"));
    sleepMillis = 0;
  }
}

void toggleSleepTimer() {
  if (sleepMillis != 0) {
    disableSleepTimer();
  } else {
    setSleepTimer();
  }
}

uint8_t pollSleep() {
  if (sleepMillis == 0) {
    return SLEEP_DISABLED;
  } else {
    if (millis() > sleepMillis) {
      return SLEEP_ACTIVATE;
    } else {
      return SLEEP_NOT_YET;
    }
  }
}

void goToSleep() {
  Serial.println(F("Going to sleep NOW!"));
  sleepMillis = 0;
  pausePlayback();
}

void setSleepLed(bool state) {
  digitalWrite(sleepStatusLedPin, state ? HIGH : LOW);
}
