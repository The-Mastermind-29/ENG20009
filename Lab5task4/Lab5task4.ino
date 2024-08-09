#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library for ST7735
#include <SPI.h>
#include "RTClib.h"
#include "SdFat.h"

#define TFT_CS 10
#define TFT_RST 6
#define TFT_DC 7
#define TFT_SCLK 13
#define TFT_MOSI 11

Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_MOSI, TFT_SCLK, TFT_RST);
RTC_DS1307 rtc;
const int buttons[] = { 2, 3, 4, 5 };

// Chip select may be constant or RAM variable.
const uint8_t SD_CS_PIN = A3;
//
// Pin numbers in templates must be constants.
const uint8_t SOFT_MISO_PIN = 12;
const uint8_t SOFT_MOSI_PIN = 11;
const uint8_t SOFT_SCK_PIN = 13;

// SdFat software SPI template
SoftSpiDriver<SOFT_MISO_PIN, SOFT_MOSI_PIN, SOFT_SCK_PIN> softSpi;

#define SD_CONFIG SdSpiConfig(SD_CS_PIN, SHARED_SPI, SD_SCK_MHZ(0), &softSpi)

bool timeToggle = false;
bool readInput = false;
bool doneReading = false;
bool stopAlarm = false;
bool startAlarm = false;
bool toggleColour = false;

DateTime alarmTime;
const char delimiter[] = ":";

//define file system for sd card
SdFs sd;
FsFile file;

const char* fileName = "lab4.txt";

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 4; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }

  tft.initR(INITR_BLACKTAB);
  tft.setRotation(3);

  rtc.begin();
  rtc.adjust(DateTime((__DATE__), (__TIME__)));

  if (!sd.begin(SD_CONFIG)) {
    Serial.println("SD card initialization failed!");
    sd.initErrorHalt();
    while (1)
      ;
  }

  // Open/create a file for writing
  if (!file.open(fileName, O_RDWR | O_CREAT)) {
    sd.errorHalt(F("open failed"));
  }

  file.close();  //release file
  Serial.println("SD Card initialized");
}
char buffer[50];

void loop() {
  DateTime dt = rtc.now();

  if (digitalRead(buttons[0]) == LOW) {
    timeToggle = !timeToggle;
    Serial.println("Toggle the time");
  }

  if (digitalRead(buttons[1]) == LOW) {
    Serial.println("Enter time in 24 hour (e.g. 15:30): ");
    readInput = true;
  }

  if (readInput) {
    readAlarmTime();
  }

  if (digitalRead(buttons[2]) == LOW) {
    stopAlarm = true;
    startAlarm = false;
    Serial.println("Stopping the alarm");
  }

  if (dt.hour() == alarmTime.hour() && dt.minute() == alarmTime.minute() && !stopAlarm) {
    Serial.println("Alarm ringing!!");
    startAlarm = true;
  }

  Serial.println(startAlarm);
  Serial.println(stopAlarm);

  if (stopAlarm) {
    //stop blinking
    tft.fillScreen(ST77XX_BLACK);
    startAlarm = false;
    stopAlarm = true;
  } else {
    if (startAlarm) {
      // blink screen
      toggleColour ? tft.fillScreen(ST77XX_YELLOW) : tft.fillScreen(ST77XX_BLACK);
      toggleColour = !toggleColour;
    }
  }

  if (digitalRead(buttons[3]) == LOW) {
    SaveDataToUSB(dt);
  }


  printMenu(timeToggle);
  delay(100);
}

void printMenu(bool isTwelveHour) {
  DateTime dt = rtc.now();
  char timeString[12];
  String text;

  if (isTwelveHour) {
    snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d %s", dt.twelveHour(), dt.minute(), dt.second(), dt.isPM() == 1 ? "PM" : "AM");
  } else {
    snprintf(timeString, sizeof(timeString), "%02d:%02d:%02d", dt.hour(), dt.minute(), dt.second());
  }
  text = timeString;
  text += "\n\n(PB1) - toggle btn 12 & 24 hour clock";
  text += "\n\n(PB2) - enter time (24h) for alarm";
  text += "\n\n(PB3) - stop alarm";
  text += "\n\n(PB4) - save time to SD";
  testdrawtext(text, ST77XX_WHITE);
}

void testdrawtext(String text, uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.setCursor(0, 20);
  tft.print(text);
}

void readAlarmTime() {
  String alarmString;

  while (alarmString == "") {
    readInput = false;
    alarmString = Serial.readString();

    doneReading = true;
  }

  if (doneReading) {
    doneReading = false;
    char* newLine = strchr(alarmString.c_str(), '\n');
    if (newLine != NULL)
      *newLine = '\0';

    char* temp;
    char* hour_str = strtok_P((char*)alarmString.c_str(), delimiter);
    if (hour_str == NULL) {
      Serial.println("Error reading hour_str");
      return;
    }
    u_int8_t hour = strtol(hour_str, &temp, 10);
    if (*temp != '\0') {
      Serial.println("Error reading hour");
      return;
    }

    char* min_str = strtok_P(NULL, delimiter);
    if (min_str == NULL) {
      Serial.println("Error reading min_str");
      return;
    }
    u_int8_t min = strtol(min_str, &temp, 10);
    if (*temp != '\0') {
      Serial.println("Error reading min");
      return;
    }

    alarmTime = DateTime(rtc.now().year(), rtc.now().month(), rtc.now().day(), hour, min, (u_int8_t)0);
    Serial.println(alarmTime.timestamp());
  }
}

void SaveDataToUSB(DateTime dt) {
  file.open(fileName, O_RDWR);
  file.rewind();  //Go to file position 0 for project use file.seekend
  file.print("Current time: ");
  file.println(dt.timestamp());

  file.print("Alarm time: ");
  file.println(alarmTime.timestamp());

  file.print("Time format: ");
  file.println(timeToggle ? "12 Hour" : "24 Hour");

  file.close();
  Serial.println("Data saved to USB");
}