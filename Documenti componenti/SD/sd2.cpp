// Demo of fgets function to read lines from a file.
#include <SdFat.h>

// SD chip select pin
const uint8_t chipSelect = SS;

SdFat sd;

SdFile file;

// Maximum line length plus space for zero byte.
const size_t LINE_DIM = 50;

char line[LINE_DIM];
//------------------------------------------------------------------------------
// store error strings in flash memory
#define error(s) sd.errorHalt_P(PSTR(s))
//------------------------------------------------------------------------------
void setup(void) {
  size_t n;
  Serial.begin(9600);
  while (!Serial) {}  // Wait for Leonardo

  Serial.println(F("Type any character to start"));
  while (Serial.read() <= 0) {}

  if (!sd.begin(chipSelect, SPI_HALF_SPEED)) sd.initErrorHalt();

  if (!file.open("TEST.TXT", O_READ)) error("open failed");

  int ln = 1;
  while ((n = file.fgets(line, sizeof(line))) > 0) {
      // Print line number.
      Serial.print(ln++);
      Serial.print(": ");
      Serial.print(line);
      if (line[n - 1] != '\n') {
        // Line is too long or last line is missing nl.
        Serial.println(F(" <-- missing nl"));
      }
  }
  Serial.println(F("\nDone"));
}
void loop(void) {}
