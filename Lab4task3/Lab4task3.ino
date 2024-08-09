#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>
#include "RTClib.h"

#define TFT_CS 10
#define TFT_RST 6
#define TFT_DC 7
#define TFT_SCLK 13
#define TFT_MOSI 11

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
RTC_DS1307 rtc;

void setup() {
  Serial.begin(9600);
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);

  rtc.begin();
  rtc.adjust(DateTime((__DATE__), (__TIME__)));

}

void loop() {
  DateTime dt = rtc.now();
  testdrawtext(dt.timestamp(), ST77XX_WHITE);
  delay(1000);
}

void testdrawtext(String text, uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.setCursor(10, 60);

  tft.print(text);
}