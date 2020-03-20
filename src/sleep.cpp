#include "headers.h"

unsigned long sleepTimer = 15UL * 60UL * 1000UL; // miliseconds
unsigned long sleepMillis = 0; // miliseconds

void setSleepTimer() {
  Serial.println("Sleep in " + String(sleepTimer / 1000UL) + "s");
  pausePlayback();
  delay(1000);
  startPlayback();
  sleepMillis = millis() + sleepTimer;
}

void disableSleepTimer() {
  if (sleepMillis != 0) {
    Serial.println(F("Disabling sleep timer"));
    pausePlayback();
    delay(200);
    startPlayback();
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

byte pollSleep() {
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
