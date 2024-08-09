int rows[] = { 13, 12, 11, 10, 9, 8, 7, 6 };
int columns[] = { 14, 15, 16, 17, 18, 19, 20, 21 };
int pushButtons[] = { 2, 3, 4, 5 };

#define POT A0

bool buttonState[4] = { false, false, false, false };

int SINWAVE[8][16] = {
  { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0 }
};

int TRIWAVE[8][16] = {
  { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }
};

int SQUAREWAVE[8][16] = {
  { 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1 },
  { 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1 }
};

int SAWWAVE[8][16] = {
  { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 },
  { 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0 },
  { 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0 },
  { 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0 },
  { 0, 0, 0, 1, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 0, 0 },
  { 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0 },
  { 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0 },
  { 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0 }
};

void setup() {
  Serial.begin(9600);

  for (int i = 0; i < 4; i++) {
    pinMode(pushButtons[i], INPUT_PULLUP);
  }

  for (int i = 0; i < 8; i++) {
    pinMode(rows[i], OUTPUT);
    pinMode(columns[i], OUTPUT);
  }

  for (int i = 0; i < 8; i++) {
    digitalWrite(rows[i], HIGH);
    digitalWrite(columns[i], LOW);
  }
}

void selectLedRow(int row) {
  for (int i = 0; i < 8; i++) {
    if (row == i) {
      digitalWrite(rows[i], LOW);  // Active row
    } else {
      digitalWrite(rows[i], HIGH);  // Inactive row
    }
  }
}

void selectLedColumns(int column, int state) {
  digitalWrite(columns[column], state);
}

void loop() {

  int mappedValue = map(analogRead(POT), 0, 1023, 2, 6);

  if (digitalRead(pushButtons[0]) == LOW || buttonState[0]) {
    setButtonState(0);
    scrollCharacters(SINWAVE, mappedValue);
  }

  if (digitalRead(pushButtons[1]) == LOW || buttonState[1]) {
    setButtonState(1);
    scrollCharacters(TRIWAVE, mappedValue);
  }

  if (digitalRead(pushButtons[2]) == LOW || buttonState[2]) {
    setButtonState(2);
    scrollCharacters(SQUAREWAVE, mappedValue);
  }

  if (digitalRead(pushButtons[3]) == LOW || buttonState[3]) {
    setButtonState(3);
    scrollCharacters(SAWWAVE, mappedValue);
  }
}

void scrollCharacters(int character[8][16], int delayTime) {
  int y[8][8] = {
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
    { 0, 0, 0, 0, 0, 0, 0, 0 },
  };

  for (int i = 0; i < 16; i++) {
    for (int j = 0; j <= i; j++) {
      int position = 8 - i + j - 1;
      for (int k = 0; k < 8; k++) {
        if (breakLoop())
          break;
        if (position >= 0 && position < 8 && j >= 0 && j < 16)
          y[k][position] = character[k][j];
      }
    }

    if (breakLoop())
      return;

    for (int counter = 0; counter < delayTime; counter++) {
      for (int a = 0; a < 8; a++) {
        for (int b = 0; b < 8; b++) {
          if (breakLoop())
            return;
          selectLedColumns(b, y[a][b]);
        }
        selectLedRow(a);
        delay(2);
      }
    }
  }
}

void setButtonState(int index) {
  for (int i = 0; i < 4; i++)
    buttonState[i] = false;
  buttonState[index] = true;
}

bool breakLoop() {
  if (digitalRead(pushButtons[0]) == LOW) {
    setButtonState(0);
    return true;
  }

  if (digitalRead(pushButtons[1]) == LOW) {
    setButtonState(1);
    return true;
  }

  if (digitalRead(pushButtons[2]) == LOW) {
    setButtonState(2);
    return true;
  }

  if (digitalRead(pushButtons[3]) == LOW) {
    setButtonState(3);
    return true;
  }

  return false;
}
