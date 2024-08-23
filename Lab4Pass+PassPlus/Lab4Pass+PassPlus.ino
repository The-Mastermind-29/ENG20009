#define LED1 11
#define LED2 10
#define LED3 9
#define BUZZER_PIN 6

int parentMenuOption = 0;
int time = 100;
int brightness = 0;
int volume = 0;

void setup() {
  Serial.begin(9600);
  printMenu();
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
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

void printMenu() {
  Serial.println("Main Menu:");
  Serial.println("1. Option 1");
  Serial.println("2. Option 2");
  Serial.println("3. Option 3");
  Serial.println("4. Option 4");
}

void printSubmenu1() {
  Serial.println("Sub Menu:");
  Serial.println("1. Toggle LED");
  Serial.println("2. Option 2");
  Serial.println("3. Return to Main Menu");
}

void printSubmenu2() {
  Serial.println("Sub Menu:");
  Serial.println("1. Increase Blinking Speed");
  Serial.println("2. Decrease Blinking Speed");
  Serial.println("3. Return to Main Menu");
}

void printSubmenu3() {
  Serial.println("Sub Menu:");
  Serial.println("1. Increase Brightness");
  Serial.println("2. Decrease Brightness");
  Serial.println("3. Return to Main Menu");
}

void printSubmenu4() {
  Serial.println("Sub Menu:");
  Serial.println("1. Increase Volume");
  Serial.println("2. Decrease Volume");
  Serial.println("3. Return to Main Menu");
}

void loop() {
  if (Serial.available()) {
    char input = Serial.read();
    switch (parentMenuOption) {
      case 1:
        switch (input) {
          case '1':
            if (digitalRead(LED1) == HIGH) {
              digitalWrite(LED1, LOW);
            } else {
              digitalWrite(LED1, HIGH);
            }
            break;
          case '2':
            Serial.println("You selected sub option 2");
            break;
          case '3':
            parentMenuOption = 0;
            printMenu();
            break;
        }
        break;
      case 2:
        switch (input) {
          case '1':
            time -= 10;
            time = constrain(time, 5, 500);

            break;
          case '2':
            time += 10;
            time = constrain(time, 5, 500);
            break;
          case '3':
            parentMenuOption = 0;
            printMenu();
            break;
        }
        break;
      case 3:
        switch (input) {
          case '1':
            brightness += 25;
            brightness = constrain(brightness, 0, 255);
            break;
          case '2':
            brightness -= 25;
            brightness = constrain(brightness, 0, 255);
            break;
          case '3':
            parentMenuOption = 0;
            printMenu();
            break;
        }
        break;
      case 4:
        switch (input) {
          case '1':
            volume += 25;
            volume = constrain(volume, 0, 255);
            break;
          case '2':
            volume -= 25;
            volume = constrain(volume, 0, 255);
            break;
          case '3':
            parentMenuOption = 0;
            printMenu();
            break;
        }
        break;
      default:
        switch (input) {
          case '1':
            Serial.println("You selected option 1");
            parentMenuOption = 1;
            printSubmenu1();
            break;
          case '2':
            Serial.println("You selected option 2");
            parentMenuOption = 2;
            printSubmenu2();
            break;
          case '3':
            Serial.println("You selected option 3");
            parentMenuOption = 3;
            printSubmenu3();
            break;
          case '4':
            Serial.println("You selected option 4");
            parentMenuOption = 4;
            printSubmenu4();
            break;
        }
        break;
    }
  }
  blink();
  controlBrightness();
  buzzerVolume();
}