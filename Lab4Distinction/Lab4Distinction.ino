#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <DFRobot_BMX160.h>

#define TFT_CS   10
#define TFT_RST  6
#define TFT_DC   7
#define TFT_SCLK 13
#define TFT_MOSI 11

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);

DFRobot_BMX160 bmx160;

void setup() {
  Serial.begin(9600);
  delay(100);
  
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);

  if (!bmx160.begin()) {
    Serial.println("BMX160 initialization failed!");
    while (1);
  }
  
  bmx160.wakeUp(); 
}

void loop() {
  sBmx160SensorData_t Omagn, Ogyro, Oaccel;
  bmx160.getAllData(&Omagn, &Ogyro, &Oaccel);

  tft.fillScreen(ST77XX_BLACK);

  Serial.print("Accelerometer: ");
  Serial.print("X: "); Serial.print(Oaccel.x);
  Serial.print("  Y: "); Serial.print(Oaccel.y);
  Serial.print("  Z: "); Serial.println(Oaccel.z);

  if (abs(Oaccel.x) > abs(Oaccel.y)) {
    if (Oaccel.x > 0) {
      tft.setRotation(2);
    } else {
      tft.setRotation(0); 
    }
  } else {
    if (Oaccel.y > 0) {
      tft.setRotation(1);
    } else {
      tft.setRotation(3);
    }
  }

  // Draw the arrow
  tft.fillTriangle(50, 82, 80, 29, 110, 82, ST77XX_RED);

  delay(100); // Adjust delay as needed
}
