#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include "RTClib.h"

#define TFT_CS 10
#define TFT_RST 6
#define TFT_DC 7
#define TFT_SCLK 13
#define TFT_MOSI 11

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
const int interruptPin = 2;
String text1;
volatile bool buttonState = false;

RTC_DS1307 rtc;

void setup(void) {
  Serial.begin(9600);

  tft.initR(INITR_BLACKTAB);  // Init ST7735S chip, black tab
  tft.setRotation(3);

  rtc.begin();
  rtc.adjust(DateTime((__DATE__), (__TIME__)));

  pinMode(interruptPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(interruptPin), changeClock, FALLING);

  // Set up the timer interrupt
  setupTimerInterrupt();
}

void loop() {
  // Nothing to do here since we're using interrupts
}

void changeClock() {
  buttonState = !buttonState;
}

void setupTimerInterrupt() {
  // Set up the generic clock (GCLK4) used to clock timers
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(1) |  // Divide the 48MHz clock source by divisor 1: 48MHz/1=48MHz
                     GCLK_GENDIV_ID(4);    // Select Generic Clock (GCLK) 4
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;  // Wait for synchronization

  GCLK->GENCTRL.reg = GCLK_GENCTRL_IDC |          // Set the duty cycle to 50/50 HIGH/LOW
                      GCLK_GENCTRL_GENEN |        // Enable GCLK4
                      GCLK_GENCTRL_SRC_DFLL48M |  // Set the 48MHz clock source
                      GCLK_GENCTRL_ID(4);         // Select GCLK4
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;  // Wait for synchronization

  // Feed GCLK4 to TC4 and TC5
  GCLK->CLKCTRL.reg = GCLK_CLKCTRL_CLKEN |      // Enable GCLK4 to TC4 and TC5
                      GCLK_CLKCTRL_GEN_GCLK4 |  // Select GCLK4
                      GCLK_CLKCTRL_ID_TC4_TC5;  // Feed the GCLK4 to TC4 and TC5
  while (GCLK->STATUS.bit.SYNCBUSY)
    ;  // Wait for synchronization

  // Set up Timer/Counter 4 (TC4) for interrupt
  TC4->COUNT16.CC[0].reg = 86000;  // Set the TC4 CC0 register as the maximum value
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY)
    ;  // Wait for synchronization

  NVIC_SetPriority(TC4_IRQn, 0);  // Set the Nested Vector Interrupt Controller (NVIC) priority for TC4 to 0 (highest)
  NVIC_EnableIRQ(TC4_IRQn);       // Connect TC4 to Nested Vector Interrupt Controller (NVIC)

  TC4->COUNT16.INTFLAG.bit.OVF = 1;   // Clear the interrupt flags
  TC4->COUNT16.INTENSET.bit.OVF = 1;  // Enable TC4 interrupts

  TC4->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024 |  // Set prescaler to 1024, 48MHz/1024 = 46.875kHz
                            TC_CTRLA_WAVEGEN_MFRQ |       // Put the timer TC4 into match frequency (MFRQ) mode
                            TC_CTRLA_ENABLE;              // Enable TC4
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY)
    ;
}

void TC4_Handler() {  // Interrupt Service Routine (ISR) for timer TC4

  // Check for overflow (OVF) interrupt
  if (TC4->COUNT16.INTFLAG.bit.OVF && TC4->COUNT16.INTENSET.bit.OVF) {
    // Clear the interrupt flag
    TC4->COUNT16.INTFLAG.bit.OVF = 1;
  }

  // Update the display based on buttonState
  if (buttonState) {
    DateTime dt = rtc.now();
    testdrawtext(dt.timestamp(), ST77XX_WHITE);
    delay(1000);
  } else {
    DateTime dt = rtc.now();
    char timeString[12];
    snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d %s", dt.twelveHour(), dt.minute(), dt.second(), dt.isPM() == 1 ? "PM" : "AM");
    text1 = timeString;
    testdrawtext(text1, ST77XX_WHITE);
  }
}

void testdrawtext(String text, uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.setCursor(10, 20);
  tft.print(text);
}
