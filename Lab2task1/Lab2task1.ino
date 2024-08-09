const int PUSHBUTTON = 2;
const int LED = 11;

bool ledOn = false;

void setup() {
  pinMode(PUSHBUTTON, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  int buttonState = digitalRead(PUSHBUTTON); // Read the state of the button
  if (buttonState == LOW) {
    while (digitalRead(PUSHBUTTON) == LOW) {
      delay(50); // Small delay to avoid bouncing
    }
    ledOn = !ledOn;
    digitalWrite(LED, ledOn);
    while (digitalRead(PUSHBUTTON) == LOW) {
      delay(50);
    }
  }
}
