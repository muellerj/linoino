void volumeUp() {
  uint8_t currentVolume = getVolume();
  setVolume(currentVolume + 1);
}

void volumeDown() {
  uint8_t currentVolume = getVolume();
  setVolume(currentVolume - 1);
}

uint8_t getVolume() {
  return EEPROM.read(256);
}

void setVolume(uint8_t vol) {
  vol = max(vol, MIN_VOLUME);
  vol = min(vol, MAX_VOLUME);
  printf("Volume: %d/%d\n", vol, MAX_VOLUME);
  EEPROM.write(256, vol);
  mp3.setVolume(vol);
}
