unsigned long sleepTimer = 100UL * 1000UL; // miliseconds
unsigned long sleepMillis = 0; // miliseconds

const byte SLEEP_DISABLED    = 0;
const byte SLEEP_NOT_YET     = 1;
const byte SLEEP_ACTIVATE    = 2;

void setSleepTimer();
void disableSleepTimer();
byte pollSleep();
void goToSleep();
