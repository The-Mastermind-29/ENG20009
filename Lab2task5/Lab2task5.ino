const int PB1 = 2;
const int PB2 = 3;
const int PB3 = 4;
const int PB4 = 5;
const int BUZZER_PIN = 6;
const int rowPins[] = { 13, 12, 11, 10, 9, 8, 7, 6 };
const int colPins[] = { 14, 15, 16, 17, 18, 19, 20, 21 };

int allowBuzzer = false;
int increment = 0;

void setup() {
  Serial.begin(9600);
  pinMode(BUZZER_PIN, OUTPUT);
  for (int i = 0; i < 8; i++) {
    pinMode(rowPins[i], OUTPUT);
  }
  for (int k = 0; k < 8; k++) {
    pinMode(colPins[k], OUTPUT);
  }
  pinMode(PB1, INPUT_PULLUP);
  pinMode(PB2, INPUT_PULLUP);
  pinMode(PB3, INPUT_PULLUP);
  pinMode(PB4, INPUT_PULLUP);
}

void loop() {
  int pbOff = digitalRead(PB1);
  int pbOn = digitalRead(PB2);
  int pbIncrease = digitalRead(PB3);
  int pbDecrease = digitalRead(PB4);

  int buzzerVolume = 0;
  if (pbOff == LOW) {
    allowBuzzer = false;
    for (int i = 0; i < 8; i++) {
      digitalWrite(colPins[i], LOW);
    }
  }

  if (pbOn == LOW) {
    allowBuzzer = true;
    increment = 0;
  }

  if (pbIncrease == LOW) {
    increment += 32;
    if (increment >= 256)
      increment = 256;

    for (int i = 0; i < increment / 32; i++) {
      digitalWrite(colPins[i], HIGH);
    }
    delay(250);
  }

  if (pbDecrease == LOW) {
    increment -= 32;
    if (increment <= 0)
      increment = 0;

    for (int i = increment / 32; i < 8; i++) {
      digitalWrite(colPins[i], LOW);
    }
    delay(250);
  }

  if (allowBuzzer) {
    buzzerVolume = increment;
  } else {
    buzzerVolume = 0;
  }
  analogWrite(BUZZER_PIN, buzzerVolume);
  delay(100);
}

void ledOff() {
  for (int i = 0; i < 8; i++) {
    digitalWrite(rowPins[i], LOW);
    digitalWrite(colPins[i], HIGH);
  }
}

void dotcycle() {
  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {

      digitalWrite(rowPins[i], HIGH);
      digitalWrite(colPins[j], LOW);
    }
  }
}
