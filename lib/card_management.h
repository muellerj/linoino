void dump_byte_array(byte *buffer, byte bufferSize);
void resetCard();
void setupCard();
bool readCard(nfcTagObject *nfcTag);
void writeCard(nfcTagObject nfcTag);
void handleKnownCard();
void handleCardReader();
byte pollCard();
