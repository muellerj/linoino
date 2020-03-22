// this object stores nfc tag data
struct nfcTagObject {
  uint32_t cookie;
  uint8_t  version;
  uint8_t  folder;
  uint8_t  mode;
  uint8_t  special;
};

extern nfcTagObject myCard;

extern bool hasCard;

#define PCS_NO_CHANGE    0 // no change detected since last pollCard()
#define PCS_NEW_CARD     1 // card with new UID detected 
#define PCS_CARD_GONE    2 // card is not reachable anymore
#define PCS_CARD_IS_BACK 3 // card was gone, and is now back again

#define MODE_RANDOM 1
#define MODE_ALBUM  2
#define MODE_PARTY  3
#define MODE_SINGLE 4
#define MODE_BOOK   5

// MFRC522
extern MFRC522 mfrc522;
extern MFRC522::MIFARE_Key key;

void setupCardReader();
void onNewCard();
void resetCard();
void setupCard(bool reset);
void forgetCard();
void rememberCard();
bool isSameCard();
bool readCard(nfcTagObject *nfcTag);
void writeCard(nfcTagObject nfcTag);
void handleKnownCard();
uint8_t pollCard();
