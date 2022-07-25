const int pins[] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36};
//const int pins[] = {35, 36};
const int pinsSize = 15;

const int WAIT = 50;
const int DROP = 50;
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
    Serial.println(pins[cnt]);
digitalWrite(pins[cnt], HIGH);
    delay(DROP);
    digitalWrite(pins[cnt], LOW);
    delay(WAIT);

  if(cnt + 1 > pinsSize - 1) {
    dir = -1;
  }
  else if (cnt - 1 < 0) {
    dir = 1;
  }
  cnt += dir;
//  Serial.println(cnt);
}
