#define ENABLE_STANDBY false

#define STDBY_DISABLED 0
#define STDBY_NOT_YET  1
#define STDBY_ACTIVATE 2

void setStandbyTimer();
void disablestandbyTimer();
uint8_t pollStandby();
void setStandby();
