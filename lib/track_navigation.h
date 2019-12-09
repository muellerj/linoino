// DFPlayer Mini
SoftwareSerial mySoftwareSerial(2, 3); // RX, TX
uint16_t numTracksInFolder;
uint16_t currentTrack;
uint16_t lastTrackFinished;


bool isPlaying();
void saveProgress();
void resetProgress();
bool isPlaying();
void nextTrack(uint16_t track);
void previousTrack();
void startPlayback();
void pausePlayback();
void resetPlayback();
