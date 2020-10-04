#include "headers.h"

uint16_t trackCount;
uint16_t currentTrack;
uint16_t lastTrackFinished;

bool setupActive = false;

bool isPlaying() { 
  return !digitalRead(busyPin);
}

uint16_t newRandomTrack(uint16_t trackCount) {
  uint16_t newTrack = random(1, trackCount + 1);
  if (newTrack == currentTrack)
    newTrack = newTrack == trackCount ? 1 : newTrack + 1;
  return newTrack;
}

void saveProgress() {
  EEPROM.write(myCard.folder, currentTrack);
}

void resetPlayback(uint16_t track) {
  uint8_t currentVolume = getVolume();
  mp3.setVolume(0);
  playTrack(track);
  delay(500);
  mp3.pause();
  mp3.setVolume(currentVolume);
}

void playTrack(uint8_t track) {
  Serial.println("Playing track " + String(track) + \
                 " from folder " + String(myCard.folder));
  mp3.playFolderTrack(myCard.folder, track);
}

void playTrack(uint8_t folder, uint8_t track) {
  Serial.println("Playing track " + String(track) + \
                 " from folder " + String(folder));
  mp3.playFolderTrack(folder, track);
}

uint16_t getTrackCount() {
  return mp3.getFolderTrackCount(myCard.folder);
}

uint8_t getFolderCount() {
  return mp3.getTotalFolderCount() - 2;
}

void playMessage(uint16_t message) {
  mp3.playMp3FolderTrack(message);
}

void pausePlayback() { 
  mp3.pause(); 
  setStandbyTimer();
}

void startPlayback() { 
  if (!hasCard()) return;
  mp3.start();
}

void nextTrack(uint16_t track, bool userChoice) {

  if (track == lastTrackFinished) return;
  lastTrackFinished = track;

  switch(myCard.mode) {
  case MODE_RANDOM:
    if (userChoice) {
      Serial.println(F("Mode random -> new random track"));
      currentTrack = newRandomTrack(trackCount);
      playTrack(currentTrack);
    } else {
      Serial.println(F("Mode random -> stop"));
      pausePlayback();
    }
    break;

  case MODE_SINGLE:
    if (userChoice) {
      Serial.println(F("Mode single -> Replay current track"));
      playTrack(currentTrack);
    } else {
      Serial.println(F("Mode single -> stop"));
      pausePlayback();
    }
    break;

  case MODE_ALBUM:
    if (currentTrack < trackCount) {
      currentTrack = currentTrack + 1;
      Serial.println("Mode album -> next track: " + String(currentTrack));
      playTrack(currentTrack);
    } else {
      currentTrack = 1;
      Serial.println(F("Mode album -> end"));
      pausePlayback();
    }
    break;

  case MODE_PARTY:
    currentTrack = newRandomTrack(trackCount);
    Serial.println("Mode party -> next random track: " + String(currentTrack));
    playTrack(currentTrack);
    break;

  case MODE_BOOK:
    if (currentTrack < trackCount) {
      currentTrack = currentTrack + 1;
      Serial.println("Mode book -> next track (saved): " + String(currentTrack));
      playTrack(currentTrack);
      saveProgress();
    } else {
      currentTrack = 1;
      Serial.println(F("Mode book -> end, resetting track to 1"));
      saveProgress();
      pausePlayback();
    }
    break;
  }
}

void previousTrack() {
  switch(myCard.mode) {
  case MODE_RANDOM:
    Serial.println(F("Mode random -> play track again"));
    break;

  case MODE_ALBUM:
    currentTrack = max(currentTrack - 1, 1);
    Serial.println(F("Mode album -> previous track"));
    break;

  case MODE_PARTY:
    Serial.println(F("Party mode -> Replay current track"));
    break;

  case MODE_SINGLE:
    Serial.println(F("Mode single -> Replay current track"));
    break;

  case MODE_BOOK:
    currentTrack = max(currentTrack - 1, 1);
    Serial.println(F("Mode book -> previous track (saved)"));
    saveProgress();
    break;
  }

  playTrack(currentTrack);
}
