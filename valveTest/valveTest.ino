const int pins[] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36};
const int pinsSize = 15;

const int WAIT = 1000;
const int DROP = 50;

void setup() {
  Serial.begin(9600);
  for(int i = 0; i < pinsSize; i++) {
    Serial.println("setup");
    pinMode(pins[i], OUTPUT);
  }
}

void loop() {
  for(int i = 0; i < pinsSize; i++) {
    Serial.print("HI : ");
    Serial.println(pins[i]);
    digitalWrite(pins[i], HIGH);
    delay(DROP);
    Serial.print("LO : ");
    Serial.println(pins[i]);
    digitalWrite(pins[i], LOW);
    delay(WAIT);
  }
}
