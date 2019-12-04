void previewSelection(int folder, int track) {
  if (folder == -1) return;
  do { delay(10); } while (isPlaying());

  if (folder == 0) {
    folder = track;
    track = 1;
  }

  mp3.playFolderTrack(folder, track);
}

int changeOption(int i, int delta, int optMin, int optMax, int previewFolder) {
  i = delta > 0 ? min(i + delta, optMax - optMin + 1) : max(i + delta, 1);
  mp3.playMp3FolderTrack((optMin - 1) + i);
  delay(1000);
  previewSelection(previewFolder, i);
  return i;
}

int promptUserSelection(int question, int optMin, int optMax, 
                        int previewFolder=-1) {
  int selection = 0;
  mp3.playMp3FolderTrack(question);

  while(true) {

    mp3.loop();

    switch(pollButtons()) {
      case BTN_PAUSE_SHORTPRESS: if (selection != 0) return selection; break;
      case BTN_UP_LONGPRESS: 
        selection = changeOption(selection, +10, optMin, optMax, previewFolder);
        break;
      case BTN_UP_SHORTPRESS:
        selection = changeOption(selection, +1, optMin, optMax, previewFolder);
        break;
      case BTN_DOWN_LONGPRESS: 
        selection = changeOption(selection, -10, optMin, optMax, previewFolder);
        break;
      case BTN_DOWN_SHORTPRESS:
        selection = changeOption(selection, -1, optMin, optMax, previewFolder);
        break;
    }
  }
}
