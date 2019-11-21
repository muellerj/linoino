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
bool hasCard = false;
byte lastCardUid[4];
byte retries;
bool lastCardWasUL;

uint8_t numberOfCards = 0;

const byte PCS_NO_CHANGE     = 0; // no change detected since last pollCard() call
const byte PCS_NEW_CARD      = 1; // card with new UID detected (had no card or other card before)
const byte PCS_CARD_GONE     = 2; // card is not reachable anymore
const byte PCS_CARD_IS_BACK  = 3; // card was gone, and is now back again

const byte MODE_HOERSPIEL = 1;
const byte MODE_ALBUM     = 2;
const byte MODE_PARTY     = 3;
const byte MODE_SINGLE    = 4;
const byte MODE_BOOK      = 5;
const byte MODE_ADMIN     = 6;

// MFRC522
MFRC522 mfrc522(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key;
bool successRead;
byte sector = 1;
byte blockAddr = 4;
byte trailerBlock = 7;
MFRC522::StatusCode status;

void dump_byte_array(byte *buffer, byte bufferSize);
void resetCard();
void setupCard();
bool readCard(nfcTagObject *nfcTag);
void writeCard(nfcTagObject nfcTag);
void handleKnownCard();
void handleCardReader();
byte pollCard();
