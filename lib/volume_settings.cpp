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

void volumeUp() {
  setVolume(getVolume() + 1);
}

void volumeDown() {
  setVolume(getVolume() - 1);
}
