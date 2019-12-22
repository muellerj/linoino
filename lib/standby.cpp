void setstandbyTimer() {
  Serial.println(F("Setting standby timer"));
  sleepAtMillis = millis() + standbyTimer;
  Serial.println("Sleeping at " + String(sleepAtMillis));
}

void disablestandbyTimer() {
  Serial.println(F("Disabling standby timer"));
  sleepAtMillis = 0;
}

void pollStandby() {
  if (sleepAtMillis != 0 && millis() > sleepAtMillis) {
    Serial.println(F("Power off NOW!"));

    digitalWrite(shutdownPin, HIGH);
    delay(500);

    // http://discourse.voss.earth/t/intenso-s10000-powerbank-automatische-abschaltung-software-only/805
    // powerdown to 27mA (powerbank switches off after 30-60s)
    mfrc522.PCD_AntennaOff();
    mfrc522.PCD_SoftPowerDown();
    mp3.sleep();

    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
    cli();  // Disable interrupts
    sleep_mode();
  }
}
