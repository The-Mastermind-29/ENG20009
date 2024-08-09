#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include "RTClib.h"
#include <DFRobot_BMX160.h>


#define TFT_CS 10
#define TFT_RST 6
#define TFT_DC 7

#define TFT_SCLK 13
#define TFT_MOSI 11

DFRobot_BMX160 bmx160;


const int PUSHBUTTONS[] = { 2, 3, 4 };

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);


bool PBSTATES[] = { false, false, false };

sBmx160SensorData_t Omagn, Ogyro, Oaccel;
//^ configuration for FAT16/FAT32 and exFAT.

void setup(void) {
  bmx160.begin();
  Serial.begin(9600);
  //Serial.print(F("Hello! ST77xx TFT Test"));
  PORT->Group[PORTA].DIRSET.reg = PORT_PA06;  // Set-up digital pin D8 as an output

  // Set up the generic clock (GCLK4) used to clock timers
  GCLK->GENDIV.reg = GCLK_GENDIV_DIV(2) |  // Divide the 48MHz clock source by divisor 1: 48MHz/1=48MHz
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

  TC4->COUNT16.CC[0].reg = 0xFFFF;  // Set the TC4 CC0 register as the maximum value
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY)
    ;  // Wait for synchronization

  //NVIC_DisableIRQ(TC4_IRQn);
  //NVIC_ClearPendingIRQ(TC4_IRQn);
  NVIC_SetPriority(TC4_IRQn, 0);  // Set the Nested Vector Interrupt Controller (NVIC) priority for TC4 to 0 (highest)
  NVIC_EnableIRQ(TC4_IRQn);       // Connect TC4 to Nested Vector Interrupt Controller (NVIC)

  TC4->COUNT16.INTFLAG.bit.OVF = 1;   // Clear the interrupt flags
  TC4->COUNT16.INTENSET.bit.OVF = 1;  // Enable TC4 interrupts

  TC4->COUNT16.CTRLA.reg |= TC_CTRLA_PRESCALER_DIV1024 |  // Set prescaler to 1024, 48MHz/1024 = 46.875kHz
                            TC_CTRLA_WAVEGEN_MFRQ |       // Put the timer TC4 into match frequency (MFRQ) mode
                            TC_CTRLA_ENABLE;              // Enable TC4
  while (TC4->COUNT16.STATUS.bit.SYNCBUSY)
    ;
  //Timer0.attachInterrupt(TCO_Handler).setFrequency(0.5).start();
  // Use this initializer if using a 1.8" TFT screen:
  tft.initR(INITR_BLACKTAB);  // Init ST7735S chip, black tab


  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);


  for (int pin : PUSHBUTTONS) {
    pinMode(pin, INPUT_PULLUP);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  /* Display the accelerometer results (accelerometer data is in m/s^2) */
  int x_peak = 0;
  int y_peak = 0;
  int z_peak = 0;
  String sensorData;


  tft.drawLine(0, 40, 160, 40, ST77XX_WHITE);
  tft.drawLine(0, 80, 160, 80, ST77XX_WHITE);
  tft.drawLine(0, 120, 160, 120, ST77XX_WHITE);

  if (digitalRead(PUSHBUTTONS[0]) == LOW) {
    PBSTATES[0] = true;
    PBSTATES[1] = false;
    PBSTATES[2] = false;
    Serial.println("Button One Clicked");
  }

  if (digitalRead(PUSHBUTTONS[1]) == LOW) {
    PBSTATES[0] = false;
    PBSTATES[1] = true;
    PBSTATES[2] = false;
    Serial.println("Button Two Clicked");
  }

  if (digitalRead(PUSHBUTTONS[2]) == LOW) {
    PBSTATES[0] = false;
    PBSTATES[1] = false;
    PBSTATES[2] = true;
    Serial.println("Button 3 Clicked");
  }

  if (PBSTATES[0]) {
    x_peak = Oaccel.x;
    y_peak = Oaccel.y;
    z_peak = Oaccel.z;
    sensorData = String("Accelerometer data");
  }

  if (PBSTATES[1]) {
    x_peak = Ogyro.x;
    y_peak = Ogyro.y;
    z_peak = Ogyro.z;
    sensorData = String("Gyroscope");
  }

  if (PBSTATES[2]) {
    x_peak = Omagn.x;
    y_peak = Omagn.y;
    z_peak = Omagn.z;
    sensorData = String("Magnetometer");
  }

  x_peak = constrain(x_peak, -20, 20);
  y_peak = constrain(y_peak, -20, 20);
  z_peak = constrain(z_peak, -20, 20);

  tft.drawLine(0, 20, 0, 20, ST77XX_RED);
  tft.drawLine(0, 60, 0, 60, ST77XX_GREEN);
  tft.drawLine(0, 100, 0, 100, ST77XX_BLUE);

  //X Values
  tft.drawLine(0, 20, 80, 20 + x_peak, ST77XX_RED);
  tft.drawLine(80, 20 + x_peak, 160, 20, ST77XX_RED);

  //Y Values
  tft.drawLine(0, 60, 80, 60 + y_peak, ST77XX_GREEN);
  tft.drawLine(80, 60 + y_peak, 160, 60, ST77XX_GREEN);



  //Z Values
  tft.drawLine(0, 100, 80, 100 + z_peak, ST77XX_BLUE);
  tft.drawLine(80, 100 + z_peak, 160, 100, ST77XX_BLUE);

  tft.drawLine(0, 20, 80, 20 + x_peak, ST77XX_BLACK);
  tft.drawLine(80, 20 + x_peak, 160, 20, ST77XX_BLACK);
  tft.drawLine(0, 60, 80, 60 + y_peak, ST77XX_BLACK);
  tft.drawLine(80, 60 + y_peak, 160, 60, ST77XX_BLACK);
  tft.drawLine(0, 100, 80, 100 + z_peak, ST77XX_BLACK);
  tft.drawLine(80, 100 + z_peak, 160, 100, ST77XX_BLACK);


  tft.setCursor(50, 122);
  tft.setTextColor(ST77XX_WHITE);
  tft.print(sensorData);


  tft.setCursor(50, 122);
  tft.setTextColor(ST77XX_BLACK);
  tft.print(sensorData);




  tft.drawLine(0, 20, 160, 20, ST77XX_BLACK);
  tft.drawLine(0, 60, 160, 60, ST77XX_BLACK);
  tft.drawLine(0, 100, 160, 100, ST77XX_BLACK);
}

void TC4_Handler()  // Interrupt Service Routine (ISR) for timer TC4
{
  // Check for overflow (OVF) interrupt
  if (TC4->COUNT16.INTFLAG.bit.OVF && TC4->COUNT16.INTENSET.bit.OVF) {

    bmx160.getAllData(&Omagn, &Ogyro, &Oaccel);

    TC4->COUNT16.INTFLAG.bit.OVF = 1;  // Clear the OVF interrupt flag
  }
}
