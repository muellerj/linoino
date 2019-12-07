#define RST_PIN 9
#define SS_PIN 10
#define buttonPause A0
#define buttonUp A1
#define buttonDown A2
#define busyPin 4
#define shutdownPin 7
#define LONG_PRESS 1000

#include <DFMiniMp3.h>
#include <EEPROM.h>
#include <JC_Button.h>
#include <MFRC522.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <avr/sleep.h>

#include "lib/debug.h"
#include "lib/setup.h"
#include "lib/standby.h"
#include "lib/button_interface.h"
#include "lib/track_navigation.h"
#include "lib/card_management.h"
#include "lib/volume_settings.h"
#include "lib/voice_menu.h"
#include "lib/mp3_init.h"

void setup() {
  mySetup();
}

void loop() {
  mp3.loop();

  switch(pollButtons()) {
    case BTN_PAUSE_LONGPRESS:  isPlaying() ? playAdvert() : resetCard(); break;
    case BTN_PAUSE_SHORTPRESS: isPlaying() ? pausePlayback() : startPlayback(); break;
    case BTN_UP_LONGPRESS:     nextTrack(random(65536)); break;
    case BTN_UP_SHORTPRESS:    volumeUp(); break;
    case BTN_DOWN_LONGPRESS:   previousTrack(); break;
    case BTN_DOWN_SHORTPRESS:  volumeDown(); break;
  }

  uint8_t now = millis();
  uint8_t timeGone = static_cast<uint8_t>(now - lastCardPoll);

  if (timeGone > minPollInterval) {
    lastCardPoll = now;

    switch (pollCard()) {
      case PCS_NEW_CARD:     onNewCard(); break;
      case PCS_CARD_GONE:    pausePlayback(); setstandbyTimer(); break;
      case PCS_CARD_IS_BACK: startPlayback(); disablestandbyTimer(); break;
    }
  }
}

#include "lib/debug.cpp"
#include "lib/setup.cpp"
#include "lib/standby.cpp"
#include "lib/button_interface.cpp"
#include "lib/voice_menu.cpp"
#include "lib/card_management.cpp"
#include "lib/volume_settings.cpp"
#include "lib/track_navigation.cpp"
