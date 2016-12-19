//กดค้างไม่ได้

byte rowPin[4] = {12, 11, 10, 9};
byte colPin[4] = {8, 7, 6, 5};  // <<< ต่อ R ไว้ด้วยแต่ละขา
char keys[4][4] = {
  {'1', '2', '3', 'A'},
  {'4', '5', '6', 'B'},
  {'7', '8', '9', 'C'},
  {'*', '0', '#', 'D'}
};

void setup() {
  Serial.begin(9600);
  Serial.print("Start");
}

void loop() {
  Serial.println(Read());
  delay(500);
}

char Read() {
  bool check_pash = true;

  while (check_pash) {                                    // จะวนLoop จนกว่าจะกด ค่าถึงจะออกมา
    for (int i = 0; i < sizeof(rowPin); i++) {
      pinMode(rowPin[i], OUTPUT);
      digitalWrite(rowPin[i], HIGH);

      for (int j = 0; j < sizeof(colPin); j++) {
        pinMode(colPin[j], INPUT);
        digitalWrite(colPin[j], LOW);
        if (digitalRead(colPin[j]) == HIGH) {
          Serial.print("i=");
          Serial.print(i);
          Serial.print("  ");
          Serial.print("j=");
          Serial.print(j);
          Serial.print("  ");
          return keys[i][j];
        }
        digitalWrite(colPin[j], LOW);
      }
      digitalWrite(rowPin[i], LOW);
    }
  }

}
