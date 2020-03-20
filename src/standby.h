#define ENABLE_STANDBY true

#define STDBY_DISABLED 0
#define STDBY_NOT_YET  1
#define STDBY_ACTIVATE 2

void setstandbyTimer();
void disablestandbyTimer();
uint8_t pollStandby();
void setStandby();
