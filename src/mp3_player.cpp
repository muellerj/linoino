#include "headers.h"

void Mp3Notify::OnError(uint16_t errorCode) {
  Serial.println("Com Error " + String(errorCode));
}

void Mp3Notify::PrintlnSourceAction(DfMp3_PlaySources source, const char* action) {
  if (source & DfMp3_PlaySources_Sd) Serial.print("SD Karte ");
  if (source & DfMp3_PlaySources_Usb) Serial.print("USB ");
  if (source & DfMp3_PlaySources_Flash) Serial.print("Flash ");
  Serial.println(action);
}

void Mp3Notify::OnPlayFinished(DfMp3_PlaySources source, uint16_t track) {
  Serial.println("End of track " + String(track));
  delay(100);
  nextTrack(track);
}

void Mp3Notify::OnPlaySourceOnline(DfMp3_PlaySources source) {
  PrintlnSourceAction(source, "online");
}

void Mp3Notify::OnPlaySourceInserted(DfMp3_PlaySources source) {
  PrintlnSourceAction(source, "ready");
}

void Mp3Notify::OnPlaySourceRemoved(DfMp3_PlaySources source) {
  PrintlnSourceAction(source, "removed");
}

SoftwareSerial mySoftwareSerial(2, 3); // RX, TX
DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(mySoftwareSerial);

void setupMp3Player() {
  mp3.begin();
  setVolume(DEFAULT_VOLUME);
}
