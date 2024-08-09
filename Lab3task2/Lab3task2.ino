// Define LED pins
const int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

const int POTENTIOMETER = A0;

void setup() {
    for (int i = 0; i < numLeds; i++) {
        pinMode(ledPins[i], OUTPUT);
    }
}

void loop() {
    int potValue = analogRead(POTENTIOMETER);

    // Map POT value to LED count (0 to 10)
    int ledCount = map(potValue, 0, 1023, 0, numLeds + 1);

    for (int i = 0; i < numLeds; i++) {
        digitalWrite(ledPins[i], i < ledCount ? HIGH : LOW);
    }
}
