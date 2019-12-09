bool isPlaying() { return !digitalRead(busyPin); }

uint16_t newRandomTrack(uint16_t trackCount) {
  uint16_t newTrack = random(1, trackCount + 1);
  if (newTrack == currentTrack)
    newTrack = newTrack == trackCount ? 1 : newTrack + 1;
  return newTrack;
}

void saveProgress() {
  EEPROM.write(myCard.folder, currentTrack);
}

void resetPlayback() {
  uint8_t currentVolume = getVolume();
  setVolume(0);
  playTrack(1);
  pausePlayback();
  setVolume(currentVolume);
}

void playTrack(uint8_t track) {
  mp3.playFolderTrack(myCard.folder, track);
}

void playTrack(uint8_t folder, uint8_t track) {
  mp3.playFolderTrack(folder, track);
}

uint16_t getTrackCount() {
  return mp3.getFolderTrackCount(myCard.folder);
}

void playMessage(uint16_t message) {
  mp3.playMp3FolderTrack(message);
}

void pausePlayback() { mp3.pause(); }
void startPlayback() { mp3.start(); }

void nextTrack(uint16_t track) {
  if (track == lastTrackFinished)
    return;

  lastTrackFinished = track;
   
  // Return early if card is unknown
  if (knownCard == false)
    return;

  switch(myCard.mode) {
  case MODE_RANDOM:
    printf("Mode random -> stop\n");
    break;

  case MODE_SINGLE:
    printf("Mode single -> stop\n");
    break;

  case MODE_ALBUM:
    if (currentTrack < trackCount) {
      currentTrack = currentTrack + 1;
      printf("Mode album -> next track: %d\n", currentTrack);
      playTrack(currentTrack);
    } else {
      currentTrack = 1;
      printf("Mode album -> end\n");
      resetPlayback();
    }
    break;

  case MODE_PARTY:
    currentTrack = newRandomTrack(trackCount);
    printf("Mode party -> next random track: %d\n", currentTrack);
    playTrack(currentTrack);
    break;

  case MODE_BOOK:
    if (currentTrack < trackCount) {
      currentTrack = currentTrack + 1;
      printf("Mode book -> next track (saved): %d\n", currentTrack);
      playTrack(currentTrack);
      saveProgress();
    } else {
      currentTrack = 1;
      printf("Mode book -> end, resetting track to 1\n");
      saveProgress();
      resetPlayback();
    }
    break;
  }
}

void previousTrack() {
  switch(myCard.mode) {
  case MODE_RANDOM:
    printf("Mode random -> play track again\n");
    break;

  case MODE_ALBUM:
    currentTrack = max(currentTrack - 1, 1);
    printf("Mode album -> previous track\n");
    break;

  case MODE_PARTY:
    printf("Party mode -> Replay current track\n");
    break;

  case MODE_SINGLE:
    printf("Mode single -> Replay current track\n");
    break;

  case MODE_BOOK:
    currentTrack = max(currentTrack - 1, 1);
    printf("Mode book -> previous track (saved)\n");
    saveProgress();
    break;
  }

  playTrack(currentTrack);
}
