// DFPlayer Mini
SoftwareSerial mySoftwareSerial(2, 3); // RX, TX
uint16_t numTracksInFolder;
uint16_t currentTrack;

bool isPlaying();
void nextTrack(uint16_t track);
void previousTrack();
