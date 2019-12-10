class __FlashStringHelper;
#define FS(string_literal) (char *)(reinterpret_cast<const __FlashStringHelper*>(PSTR(string_literal)))

void dump_byte_array(byte *buffer, byte bufferSize);
int serial_putchar(char c, FILE* f);
