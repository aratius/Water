const int pins[] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36};
//const int pins[] = {35, 36};
const int pinsSize = 15;

const int WAIT = 100;
const int DROP = 10;

int cnt = -999;
int dir = 1;

void setup() {
  Serial.begin(9600);
  for(int i = 0; i < pinsSize; i++) {
    Serial.println("setup");
    pinMode(pins[i], OUTPUT);
  }

  cnt = 0;
  Serial.print("hoge");
  Serial.println(cnt);
}

void loop() {
  bool even = cnt % 2 == 0;
  for(int i = 0; i < pinsSize; i++) {
    if(even  && i % 2 == 0) {
      digitalWrite(pins[i], HIGH);
    } else if (!even && i % 2 == 1) {
      digitalWrite(pins[i], HIGH);
    }
  }
  
  delay(DROP);

  for(int i = 0; i < pinsSize; i++) {
    digitalWrite(pins[i], LOW);
  }
  delay(WAIT);

  cnt ++;
}
