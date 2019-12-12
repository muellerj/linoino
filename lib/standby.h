unsigned long standbyTimer = 5 * 60 * 1000; // miliseconds
unsigned long sleepAtMillis = 0; // miliseconds

void setstandbyTimer();
void disablestandbyTimer();
void pollStandby();
