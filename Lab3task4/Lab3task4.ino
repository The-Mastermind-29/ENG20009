//SETUP OF ARDUINO BOARD
#define POTENTIOMETER A0
#define LDR A1
#define BUZZER_PIN 6
#define dipSwitch1 2
#define dipSwitch2 3
#define dipSwitch3 4
#define dipSwitch5 5

//NOTES
#define NOTE_C4  262
#define NOTE_E4  330
#define NOTE_G4  392
#define NOTE_D4  294
#define NOTE_F4  349
#define NOTE_A4  440
#define NOTE_B4  494

int buzzerVolume = 0;
int melody[] = {NOTE_C4, NOTE_E4, NOTE_G4, NOTE_E4, NOTE_C4, NOTE_G4, NOTE_E4};
int melody1[] = {NOTE_C4, NOTE_D4, NOTE_E4, NOTE_F4, NOTE_G4, NOTE_A4, NOTE_B4};
int noteDuration;

void setup(){
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(dipSwitch1, INPUT_PULLUP);
}

void loop() {
  int switchState1 = digitalRead(dipSwitch1);
  int switchState2 = digitalRead(dipSwitch2);
  if (switchState1 == LOW) {
    for (int i = 0; i < 7; i++) {
      if (digitalRead(dipSwitch1) == HIGH) {
        break;
      }
      if (digitalRead(dipSwitch2) == LOW) {
        break;
      }
      tone(BUZZER_PIN, melody[i] + buzzerVolume, noteDuration);
      int analogValue = analogRead(POTENTIOMETER);
      int ldrValue = analogRead(LDR);
      noteDuration = map(analogValue, 0, 1023, 750, 50);
      if (ldrValue <= 100) {
        buzzerVolume = 200;
      } else {
        buzzerVolume = -150;
      }
      delay(noteDuration);
      noTone(BUZZER_PIN);
      delay(1);
    }
  }
  if (switchState2 == LOW) {
    for (int i = 0; i < 7; i++) {
      if (digitalRead(dipSwitch2) == HIGH) {
        break;
      }
      tone(BUZZER_PIN, melody1[i] + buzzerVolume, noteDuration);
      int analogValue = analogRead(POTENTIOMETER);
      int ldrValue = analogRead(LDR);
      noteDuration = map(analogValue, 0, 1023, 750, 50);
      if (ldrValue <= 100) {
        buzzerVolume = 200;
      } else {
        buzzerVolume = -150;
      }
      delay(noteDuration);
      noTone(BUZZER_PIN);
      delay(1);
    }
  } 
  else {
    noTone(BUZZER_PIN);
  }
}



