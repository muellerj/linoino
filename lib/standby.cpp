void setstandbyTimer() {
  Serial.println("Standby in " + String(standbyTimer / 1000UL) + "s");
  standbyAtMillis = millis() + standbyTimer;
}

void disablestandbyTimer() {
  if (standbyAtMillis != 0) {
    Serial.println(F("Disabling standby timer"));
    standbyAtMillis = 0;
  }
}

byte pollStandby() {
  if (standbyAtMillis == 0) {
    return STDBY_DISABLED;
  } else {
    if (millis() > standbyAtMillis) {
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
