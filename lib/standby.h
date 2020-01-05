unsigned long standbyTimer = 100 * 1000; // miliseconds
unsigned long standbyAtMillis = 0; // miliseconds

const byte STDBY_DISABLED    = 0;
const byte STDBY_NOT_YET     = 1;
const byte STDBY_ACTIVATE    = 2;

void setstandbyTimer();
void disablestandbyTimer();
byte pollStandby();
void setStandby();
