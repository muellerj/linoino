const byte BTN_NOTHING          = 0;
const byte BTN_UP_LONGPRESS     = 1;
const byte BTN_UP_SHORTPRESS    = 2;
const byte BTN_DOWN_LONGPRESS   = 3;
const byte BTN_DOWN_SHORTPRESS  = 4;
const byte BTN_PAUSE_LONGPRESS  = 5;
const byte BTN_PAUSE_SHORTPRESS = 6;

Button pauseButton(buttonPause);
Button upButton(buttonUp);
Button downButton(buttonDown);

bool ignorePauseButton = false;
bool ignoreUpButton    = false;
bool ignoreDownButton  = false;

int pollButtons();
void volumeDown();
void volumeUp();
