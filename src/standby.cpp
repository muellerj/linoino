#include "headers.h"

unsigned long standbyTimer = 100UL * 1000UL; // miliseconds
unsigned long standbyMillis = 0; // miliseconds

void setStandbyTimer() {
  if (ENABLE_STANDBY) {
    Serial.println("Standby in " + String(standbyTimer / 1000UL) + "s");
    standbyMillis = millis() + standbyTimer;
  }
}

void disablestandbyTimer() {
  if (standbyMillis != 0) {
    Serial.println(F("Disabling standby timer"));
    standbyMillis = 0;
  }
}

uint8_t pollStandby() {
  if (standbyMillis == 0 || !ENABLE_STANDBY) {
    return STDBY_DISABLED;
  } else {
    if (millis() > standbyMillis) {
      return STDBY_ACTIVATE;
    } else {
      return STDBY_NOT_YET;
    }
  }
}

void setStandby() {
  Serial.println(F("Going to Standby NOW!"));
  digitalWrite(shutdownPin, HIGH);

  delay(500);

  mfrc522.PCD_AntennaOff();
  mfrc522.PCD_SoftPowerDown();
  mp3.sleep();

  set_sleep_mode(SLEEP_MODE_PWR_DOWN);
  cli();
  sleep_mode();
}
