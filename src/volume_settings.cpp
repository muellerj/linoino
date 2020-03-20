#include "headers.h"

uint8_t getVolume() {
  return EEPROM.read(256);
}

void setVolume(uint8_t vol) {
  vol = max(vol, MIN_VOLUME);
  vol = min(vol, MAX_VOLUME);
  Serial.println("Volume: " + String(vol) + "/" + String(MAX_VOLUME));
  EEPROM.write(256, vol);
  mp3.setVolume(vol);
}

void volumeUp() {
  getVolume() < MAX_VOLUME ? setVolume(getVolume() + 1) : setVolume(MAX_VOLUME);
}

void volumeDown() {
  getVolume() > 0 ? setVolume(getVolume() - 1) : setVolume(0);
}
