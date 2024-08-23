// Define LED pins
const int ledPins[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
const int numLeds = sizeof(ledPins) / sizeof(ledPins[0]);

// Analog input pin for LDR
const int ldrPin = A1;

void setup() {
    // Initialize LED pins as OUTPUT
    for (int i = 0; i < numLeds; i++) {
        pinMode(ledPins[i], OUTPUT);
    }
}

void loop() {
    int ldrValue = analogRead(ldrPin);

    if (ldrValue < 100) { 
        for (int i = 0; i < numLeds; i++) {
            digitalWrite(ledPins[i], HIGH);
        }
    } else {
        // Otherwise, turn off all LEDs
        for (int i = 0; i < numLeds; i++) {
            digitalWrite(ledPins[i], LOW);
        }
    }
}
