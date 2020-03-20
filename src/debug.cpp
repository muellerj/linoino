#include "headers.h"

// Helper routine to dump a byte array as hex values to Serial
void dump_byte_array(uint8_t *buffer, uint8_t bufferSize) {
  for (uint8_t i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

void noOp() {}
