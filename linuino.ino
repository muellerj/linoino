#include <DFMiniMp3.h>
#include <EEPROM.h>
#include <JC_Button.h>
#include <MFRC522.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include <avr/sleep.h>

#define RST_PIN 9
#define SS_PIN 10
#define buttonPause A0
#define buttonUp A1
#define buttonDown A2
#define busyPin 4
#define shutdownPin 7
#define LONG_PRESS 1000

// DFPlayer Mini
SoftwareSerial mySoftwareSerial(2, 3); // RX, TX
uint16_t numTracksInFolder;
uint16_t currentTrack;

// this object stores nfc tag data
struct nfcTagObject {
  uint32_t cookie;
  uint8_t version;
  uint8_t folder;
  uint8_t mode;
  uint8_t special;
};

nfcTagObject myCard;

bool knownCard = false;

#include "lib/standby.h"
#include "lib/button_interface.h"
#include "lib/track_navigation.h"
#include "lib/card_management.h"
#include "lib/voice_menu.h"

// implement a notification class,
// its member methods will get called
//
class Mp3Notify {
public:
  static void OnError(uint16_t errorCode) {
    // see DfMp3_Error for code meaning
    Serial.println();
    Serial.print("Com Error ");
    Serial.println(errorCode);
  }
  static void OnPlayFinished(uint16_t track) {
    Serial.print("Track beendet");
    Serial.println(track);
    delay(100);
    nextTrack(track);
  }
  static void OnCardOnline(uint16_t code) {
    Serial.println(F("SD Karte online "));
  }
  static void OnCardInserted(uint16_t code) {
    Serial.println(F("SD Karte bereit "));
  }
  static void OnCardRemoved(uint16_t code) {
    Serial.println(F("SD Karte entfernt "));
  }
  static void OnUsbOnline(uint16_t code) {
      Serial.println(F("USB online "));
  }
  static void OnUsbInserted(uint16_t code) {
      Serial.println(F("USB bereit "));
  }
  static void OnUsbRemoved(uint16_t code) {
    Serial.println(F("USB entfernt "));
  }
};

static DFMiniMp3<SoftwareSerial, Mp3Notify> mp3(mySoftwareSerial);

// Leider kann das Modul keine Queue abspielen.
static uint16_t _lastTrackFinished;

// Time to go to sleep
long standbyTimer = 5; // Minutes
unsigned long sleepAtMillis = 0;

// MFRC522
MFRC522 mfrc522(SS_PIN, RST_PIN); // Create MFRC522
MFRC522::MIFARE_Key key;
bool successRead;
byte sector = 1;
byte blockAddr = 4;
byte trailerBlock = 7;
MFRC522::StatusCode status;

Button pauseButton(buttonPause);
Button upButton(buttonUp);
Button downButton(buttonDown);
bool ignorePauseButton = false;
bool ignoreUpButton = false;
bool ignoreDownButton = false;

static bool hasCard = false;

static byte lastCardUid[4];
static byte retries;
static bool lastCardWasUL;

const byte PCS_NO_CHANGE     = 0; // no change detected since last pollCard() call
const byte PCS_NEW_CARD      = 1; // card with new UID detected (had no card or other card before)
const byte PCS_CARD_GONE     = 2; // card is not reachable anymore
const byte PCS_CARD_IS_BACK  = 3; // card was gone, and is now back again

uint8_t numberOfCards = 0;

void setup() {

  Serial.begin(115200); // Debug via Serial interface
  randomSeed(analogRead(A0)); // Zufallsgenerator initialisieren

  Serial.println(F("TonUINO Version 2.0"));
  Serial.println(F("(c) Thorsten Voß"));

  // Knöpfe mit PullUp
  pinMode(buttonPause, INPUT_PULLUP);
  pinMode(buttonUp, INPUT_PULLUP);
  pinMode(buttonDown, INPUT_PULLUP);

  // Busy Pin
  pinMode(busyPin, INPUT);

  // DFPlayer Mini initialisieren
  mp3.begin();
  mp3.setVolume(15);

  // NFC Leser initialisieren
  SPI.begin();        // Init SPI bus
  mfrc522.PCD_Init(); // Init MFRC522
  mfrc522.PCD_DumpVersionToSerial(); // Show details of PCD - MFRC522 Card Reader
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  // Reset all cards (all three buttons)
  if (digitalRead(buttonPause) == LOW && 
      digitalRead(buttonUp)    == LOW &&
      digitalRead(buttonDown)  == LOW) {
    Serial.println(F("Reset -> EEPROM wird gelöscht"));
    for (int i = 0; i < EEPROM.length(); i++) {
      EEPROM.write(i, 0);
    }
  }
}

void loop() {

  do {
    mp3.loop();
    handleButtonPresses();
  } while (!mfrc522.PICC_IsNewCardPresent());

  if (!mfrc522.PICC_ReadCardSerial())
    return;

  handleCardReader();
}

#include "lib/standby.c"
#include "lib/button_interface.c"
#include "lib/voice_menu.c"
#include "lib/card_management.c"
#include "lib/track_navigation.c"
