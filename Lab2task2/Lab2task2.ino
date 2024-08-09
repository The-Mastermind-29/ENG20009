#define LED1 11
#define LED2 10
#define LED3 9
#define BUZZER_PIN 6

int time = 250;
int brightness = 0;
int volume = 0;
const int PUSHBUTTON1 = 2;
const int PUSHBUTTON2 = 3;
const int PUSHBUTTON3 = 4;
const int PUSHBUTTON4 = 5;
bool ledOn = false;

void setup() {
  Serial.begin(9600);
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  analogWrite(LED3, 0);
}

void controlBrightness() {
  analogWrite(LED3, brightness);
}

void buzzerVolume() {
  analogWrite(BUZZER_PIN, volume);
}

void blink() {
  digitalWrite(LED2, HIGH);
  delay(time);
  digitalWrite(LED2, LOW);
  delay(time);
}

void buttonOne() {
  int buttonState1 = digitalRead(PUSHBUTTON1);
  if (buttonState1 == LOW) {
    while (digitalRead(PUSHBUTTON1) == LOW) {
      delay(50);  // Small delay to avoid bouncing
    }
    ledOn = !ledOn;
    digitalWrite(LED1, ledOn);
    while (digitalRead(PUSHBUTTON1) == LOW) {
      delay(50);
    }
  }
}

void buttonTwo() {
  int buttonState2 = digitalRead(PUSHBUTTON2);
  if (buttonState2 == LOW) {
    time -= 10;
    time = constrain(time, 5, 500);
  }
}

void buttonThree() {
  int buttonState3 = digitalRead(PUSHBUTTON3);
  if (buttonState3 == LOW) {
    time += 10;
    time = constrain(time, 5, 500);
  }
}

void buttonFour() {
  int buttonState4 = digitalRead(PUSHBUTTON4);
  if (buttonState4 == LOW) {
    brightness += 25;
    brightness = constrain(brightness, 0, 255);
  }
}

void loop() {
  buttonOne();
  buttonTwo();
  buttonThree();
  buttonFour();
  controlBrightness();
  blink();
}
