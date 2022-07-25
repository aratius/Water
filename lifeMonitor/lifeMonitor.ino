int pins[] = {34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48};

void setup() {
  Serial.begin(115200);
  for(int i = 0; i < 15; i ++) {
   pinMode(pins[i], OUTPUT); 
  }
}

void loop() {
  for(int i = 0; i < 15; i ++) {
    digitalWrite(pins[i], HIGH);
    Serial.println(pins[i]);
    delay(100);
    digitalWrite(pins[i], LOW);
    delay(300);
  }
  delay(600);
}
