const int POTENTIOMETER_PIN = A0;
const int BUZZER_PIN = 6;

void setup() {
    pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {
    int analogValue = analogRead(POTENTIOMETER_PIN);
    int buzzerVolume = map(analogValue, 0, 1023, 0, 255); 
    analogWrite(BUZZER_PIN, buzzerVolume); 
}
