int pins[] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36};
int lines[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int beforeLines[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

void setup() {
  Serial.begin(115200);

  for(int i = 0; i < 15; i++) {
    int line = lines[i];
    // pinMode(line, OUTPUT);
  }
}

void loop() {
  printCurrent();

  int _delay = 0;
  
  // ここからそれぞれのパターン
  // コメントアウトで切り替え
  // 必要なdelayを返してかつステータスを更新する関数
  _delay = zigzag();
  
  compareLines();
  updateStatus();
  delay(_delay);
}

/**
 * 現在の状態を表示
 */
void printCurrent() {
  for(int i = 0;i < 15; i++) {
    if(lines[i] == 0) Serial.print("_");
    else Serial.print("0");
    Serial.print(" ");
  }
  Serial.println();
}

/**
 * ステータスを比べて変化があればdigitalzWriteする
 */
void compareLines() {
  for(int i = 0; i < 15; i++) {
    int crr = lines[i];
    int last = beforeLines[i];
    // TODO: 状態が変化したらHIGHとLOWを切り替え
    // digitalWrite(pins[i], lines[i]);

  }
  
}

void updateStatus() {
  for(int i = 0; i < 15; i++) {
    beforeLines[i] = lines[i];
  }
}

int zigzagCrrIndex = 0;
int zigzagDir = 1;
int zigzag() {
  int crr = zigzagCrrIndex;
  int dir = zigzagDir;

  if((crr <= 0 && dir == -1) || (crr >= 14 && dir == 1)) dir *= -1;
  crr += dir;
  
  for(int i = 0; i < 15; i++) {
    if(crr == i) lines[i] = 1;
    else lines[i] = 0;
  }

  zigzagCrrIndex = crr;
  zigzagDir = dir;
  return 100;
}
