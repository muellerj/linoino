// this object stores nfc tag data
struct nfcTagObject {
  uint32_t cookie;
  uint8_t version;
  uint8_t folder;
  uint8_t mode;
  uint8_t special;
};

nfcTagObject myCard;

static bool knownCard = false;
static bool hasCard = false;
static byte lastCardUid[4];
static byte retries;
static bool lastCardWasUL;
const byte PCS_NO_CHANGE     = 0; // no change detected since last pollCard() call
const byte PCS_NEW_CARD      = 1; // card with new UID detected (had no card or other card before)
const byte PCS_CARD_GONE     = 2; // card is not reachable anymore
const byte PCS_CARD_IS_BACK  = 3; // card was gone, and is now back again
uint8_t numberOfCards = 0;

const byte MODE_HOERSPIEL = 1;
const byte MODE_ALBUM     = 2;
const byte MODE_PARTY     = 3;
const byte MODE_SINGLE    = 4;
const byte MODE_BOOK      = 5;
const byte MODE_ADMIN     = 6;

void dump_byte_array(byte *buffer, byte bufferSize);
void resetCard();
void setupCard();
bool readCard(nfcTagObject *nfcTag);
void writeCard(nfcTagObject nfcTag);
void handleKnownCard();
void handleCardReader();
byte pollCard();
