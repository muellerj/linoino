#include "Arduino.h"

#define buttonPause A0
#define buttonUp A1
#define buttonDown A2
#define busyPin 4
#define shutdownPin 7
#define mfrcRstPin 9
#define mfrcSsPin 10

#include "DFMiniMp3.h"
#include "EEPROM.h"
#include "JC_Button.h"
#include "MFRC522.h"
#include "SPI.h"
#include "SoftwareSerial.h"
#include "avr/sleep.h"

#include "debug.h"
#include "setup.h"
#include "standby.h"
#include "sleep.h"
#include "button_interface.h"
#include "track_navigation.h"
#include "card_management.h"
#include "volume_settings.h"
#include "voice_menu.h"
#include "mp3_init.h"
