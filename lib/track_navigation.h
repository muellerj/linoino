// DFPlayer Mini
SoftwareSerial mySoftwareSerial(2, 3); // RX, TX
uint16_t trackCount;
uint16_t currentTrack;
uint16_t lastTrackFinished;


bool isPlaying();
void saveProgress();

void nextTrack(uint16_t track);
void previousTrack();

void playTrack(uint8_t track);
void playTrack(uint8_t folder, uint8_t track);

void playMessage(uint16_t track);

uint16_t getTrackCount();

void startPlayback();
void pausePlayback();
void resetPlayback(uint16_t track);
