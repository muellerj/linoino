#define SLEEP_DISABLED 1
#define SLEEP_NOT_YET  1
#define SLEEP_ACTIVATE 2

void setSleepTimer();
void disableSleepTimer();
void toggleSleepTimer();
uint8_t pollSleep();
void goToSleep();
