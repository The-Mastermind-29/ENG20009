#include <OneWire.h>
#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>

OneWire ds(8);
byte addr[] = { 0x2D, 0x7F, 0x6A, 0x68, 0x40, 0x0, 0x0, 0x7A };

#define TFT_CS 10
#define TFT_RST 6
#define TFT_DC 7

#define TFT_SCLK 13
#define TFT_MOSI 11

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup() {
  Serial.begin(9600);
  delay(1000);
  while (!Serial) {}

  String studentId = "104541189";
  writeToEEPROM(studentId);

  String EEPROMString = readFromEEPROM(studentId.length());
  Serial.println(EEPROMString);

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);

  EEPROMString.remove(studentId.length());

  Serial.println(EEPROMString);
  drawText(EEPROMString);
}

void loop() {
}

// Function to write a string to the DS28E07 EEPROM
void writeToEEPROM(String data) {
  // Convert the input string to a char array
  char dataChars[30];
  data.toCharArray(dataChars, 30);

  // Write the data to the EEPROM
  ds.reset();         //reset device
  ds.select(addr);    //select address of device to talk to
  ds.write(0x0F, 1);  // Write ScratchPad       Configuration/mode bytes can be in datasheet
  ds.write(0x01, 1);  //TA1 addresses           TA (Target Address) can also be found in the datasheet
  ds.write(0x09, 1);  //TA2 addresses
  for (int i = 0; i < data.length(); i++) {
    ds.write(dataChars[i], 1);
  }
  ds.reset();
  ds.select(addr);
  ds.write(0x0F, 1);  // Copy ScratchPad
}

// Function to read a string from the DS28E07 EEPROM
String readFromEEPROM(int length) {
  byte dataBytes[30];
  ds.reset();
  ds.select(addr);
  ds.write(0xAA);  // Read Scratchpad
  for (int i = 0; i < length + 4; i++) {
    dataBytes[i] = ds.read();
  }
  return String((char*)dataBytes).substring(3);
}

void drawText(String text) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(ST77XX_WHITE);
  tft.setTextWrap(true);
  tft.setCursor(15, 60);
  tft.print(text);
}