#include <Adafruit_GFX.h>   
#include <Adafruit_ST7735.h>  

#define TFT_CS 10
#define TFT_RST 6
#define TFT_DC 7
#define TFT_SCLK 13
#define TFT_MOSI 11

const int PUSHBUTTON = 2; 

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

void setup() { 
  pinMode(PUSHBUTTON, INPUT_PULLUP); 
  attachInterrupt(digitalPinToInterrupt(PUSHBUTTON), DrawSymbol, FALLING); 
  tft.initR(INITR_BLACKTAB);
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextSize(5);
  tft.setTextColor(ST77XX_WHITE); 
}

void loop() {
  // Loop is not used as we are using interrupt.
}

void DrawSymbol() {
  tft.fillScreen(ST77XX_BLACK);
  tft.setCursor(64, 80);
  tft.print("#");
}
