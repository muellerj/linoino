/*
 * EEPROM Organisation
 *
 * 0-255: Progress in respective folders
 *   256: Volume
 *
 */

void mySetup() {
  Serial.begin(115200); // Debug via Serial interface
  randomSeed(analogRead(A0)); // Initalize random number generator

	// Set up stdoud
	fdev_setup_stream(&serial_stdout, serial_putchar, NULL, _FDEV_SETUP_WRITE);
	stdout = &serial_stdout;

  printf("TonUINO Version 2.0\n");
  printf("(c) Thorsten Voß\n");

  // Knöpfe mit PullUp
  pinMode(buttonPause, INPUT_PULLUP);
  pinMode(buttonUp, INPUT_PULLUP);
  pinMode(buttonDown, INPUT_PULLUP);

  // Busy Pin
  pinMode(busyPin, INPUT);

  // Initialize DFPlayer Mini
  mp3.begin();
  setVolume(DEFAULT_VOLUME);

  // Initialize NFC reader
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD MFRC522 Card Reader
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  // Reset all cards (all three buttons)
  if (digitalRead(buttonPause) == LOW && 
      digitalRead(buttonUp)    == LOW &&
      digitalRead(buttonDown)  == LOW) {
    printf("Reset -> EEPROM wird gelöscht\n");
    for (int i = 0; i < EEPROM.length(); i++) {
      EEPROM.write(i, 0);
    }
  }
}
