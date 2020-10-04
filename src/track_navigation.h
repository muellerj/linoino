// DFPlayer Mini
extern uint16_t trackCount;
extern uint16_t currentTrack;
extern uint16_t lastTrackFinished;

extern bool setupActive;

bool isPlaying();
void saveProgress();

void nextTrack(uint16_t track, bool userChoice);
void previousTrack();

void playTrack(uint8_t track);
void playTrack(uint8_t folder, uint8_t track);

void playMessage(uint16_t track);

uint16_t getTrackCount();
uint8_t getFolderCount();

void startPlayback();
void pausePlayback();
void resetPlayback(uint16_t track);
