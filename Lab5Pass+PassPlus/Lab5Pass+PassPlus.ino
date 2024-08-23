#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>

#define TFT_CS 10
#define TFT_RST 6
#define TFT_DC 7

#define TFT_SCLK 13
#define TFT_MOSI 11

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

const char studentDetails1[] PROGMEM = "104541189";
const char studentDetails2[] PROGMEM = "Vishnu Hathiwala";
const char studentDetails3[] PROGMEM = "ENG20009 Engineering Technology Inquiry Project";
const char studentDetails4[] PROGMEM = "Semester 2, 2024";

const char* const studentDetails[] PROGMEM = { studentDetails1, studentDetails2, studentDetails3, studentDetails4 };
char buffer[50];

void setup() {
  Serial.begin(9600);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);

  for (int i = 0; i < 4; i++) {
    tft.fillScreen(ST77XX_BLACK);
    strcpy_P(buffer, (char*)pgm_read_word(&(studentDetails[i])));  // Necessary casts and dereferencing, just copy.
    Serial.println(buffer);
    testdrawtext(buffer, ST77XX_WHITE);
    delay(1000);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
}

void testdrawtext(String text, uint16_t color) {
  int length = text.length();
  char t[50];
  tft.setCursor(0, 60);

  for (int i = 0; i < 200; i++) {
    tft.fillScreen(ST77XX_BLACK);
    tft.setTextColor(color);
    tft.setTextWrap(true);

    if (tft.getCursorX() < 153) {
      tft.setCursor(i, 60);

      if (i < length) {
        for (int j = 0; j <= i; j++) {
          t[j] = text[length - i + j - 1];
        }
        t[i + 1] = '\0';
      } else {
        i = i + 6;
      }

      tft.print(t);
      Serial.println(tft.getCursorX());
    } else {
      for (int j = length; j >= -1; j--) {
        tft.fillScreen(ST77XX_BLACK);
        tft.setTextColor(color);
        tft.setTextWrap(true);

        tft.setCursor(154 - (j * 6), 60);
        strcpy(t, text.c_str());
        t[j + 1] = '\0';
        tft.print(t);
        delay(60);
        Serial.println(tft.getCursorX());
      }
      return;
    }
  }
}