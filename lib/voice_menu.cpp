void previewSelection(int folder, int track) {
  if (folder == -1) return;
  do { delay(10); } while (isPlaying());

  if (folder == 0) {
    folder = track;
    track = 1;
  }

  playTrack(folder, track);
}

int changeOption(int i, int delta, int optMin, int optMax, int folder) {
  i = delta > 0 ? min(i + delta, optMax - optMin + 1) : max(i + delta, 1);
  playMessage((optMin - 1) + i);
  delay(1000);
  previewSelection(folder, i);
  return i;
}

int promptUserSelection(int question, int optMin, int optMax, int folder=-1) {
  int selection = 0;
  int delta     = 0;
  playMessage(question);

  while(true) {

    mp3.loop();
    delta = 0;

    switch(pollButtons()) {
      case BTN_PAUSE_SHORTPRESS: if (selection != 0) return selection; break;
      case BTN_UP_LONGPRESS:     delta = +10; break;
      case BTN_UP_SHORTPRESS:    delta =  +1; break;
      case BTN_DOWN_LONGPRESS:   delta = -10; break;
      case BTN_DOWN_SHORTPRESS:  delta =  -1; break;
    }

    if (delta != 0)
      selection = changeOption(selection, delta, optMin, optMax, folder);
  }
}
