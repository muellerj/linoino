// DFPlayer Mini
SoftwareSerial mySoftwareSerial(2, 3); // RX, TX
uint16_t trackCount;
uint16_t currentTrack;
uint16_t lastTrackFinished;


bool isPlaying();
void saveProgress();
bool isPlaying();
void nextTrack(uint16_t track);
void playTrack(uint8_t track);
void playTrack(uint8_t folder, uint8_t track);
void playMessage(uint16_t track);
uint16_t getTrackCount();
void previousTrack();
void startPlayback();
void pausePlayback();
void resetPlayback();
