#define PIN_LEN 15
#define OPEN_TIME 50
int pins[] = {22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36};
int lines[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // 状態 0: 閉じている, 1: 瞬間開く, 2: 持続的に開く 2は自分で閉じる必要あり
int beforeLines[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // 前回の状態
float openedTime[] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};  // 最後に開いた時間

void setup() {
  Serial.begin(115200);

  for(int i = 0; i < PIN_LEN; i++) {
    int line = lines[i];
    // pinMode(line, OUTPUT);
  }
}

void loop() {
  int _delay = 0;
  
  // ここからそれぞれのパターン
  // コメントアウトで切り替え
  // 必要なdelayを返してかつステータスを更新する関数
  // _delay = zigzag();
  // _delay = sinWave();
  // _delay = all();
  _delay = toggle();
  
  compareLines();
  checkScheduledStatus();
  printCurrent();
  updateStatus();
  delay(_delay);
}


/**
 * 現在の状態を表示
 */
void printCurrent() {
  for(int i = 0;i < PIN_LEN; i++) {
    if(lines[i] == 0) Serial.print("_");
    else Serial.print("■");
    Serial.print(" ");
  }
  Serial.println();
}


/**
 * ステータスを比べて変化があればdigitalzWriteする
 */
void compareLines() {
  for(int i = 0; i < PIN_LEN; i++) {
    int crr = lines[i];
    int last = beforeLines[i];
    // TODO: 状態が変化したらHIGHとLOWを切り替え
     
    if(last == 0 && (crr == 1 || crr == 2)) {
      dropWater(i);
    }
    
  }
  
}


/**
 * 状態更新
 */
void updateStatus() {
  for(int i = 0; i < PIN_LEN; i++) {
    beforeLines[i] = lines[i];
    if(lines[i] == 1) lines[i] = 0;
  }
}


/**
 * 水を落とす
 * OFF時間のスケジューリング
 */
void dropWater(int i) {
  // digitalWrite(pins[i], lines[i]);
  openedTime[i] = (float)millis();
}


/**
 * OFFにすべき弁を閉じる
 */
void checkScheduledStatus() {
  for(int i = 0; i < PIN_LEN; i++) {
    if(lines[i] == 1 && ((float)millis() - openedTime[i]) > OPEN_TIME) {
      lines[i] = 0;
    }
  }
}


/**
 * ----- ジグザグ -----
 */
int zigzagCrrIndex = 0;
int zigzagDir = 1;
int zigzag() {
  int crr = zigzagCrrIndex;
  int dir = zigzagDir;

  if((crr <= 0 && dir == -1) || (crr >= 14 && dir == 1)) dir *= -1;
  crr += dir;
  
  for(int i = 0; i < PIN_LEN; i++) {
    if(crr == i) lines[i] = 1;
  }

  zigzagCrrIndex = crr;
  zigzagDir = dir;
  return 100;
}


/**
 * ----- sin -----
 */
int sinDir = 1;
float sinLast = 0;
int sinWave() {
    int dir = sinDir;
    unsigned long ms = millis();
    float t = (float)ms * 0.006;
    float sinCrr = (sin(t) + 1) * 0.5 * (float)PIN_LEN;

    for(int i = 0; i < PIN_LEN; i++) {
      float pos = (float)i + 0.5;
      
      if(
        dir == 1 && (pos > sinLast && pos <= sinCrr) ||
        dir == -1 && (pos < sinLast && pos >= sinCrr)
      ) {
        lines[i] = 1;
        if((i == 0 && dir == -1) || (i == PIN_LEN - 1 && dir == 1)) dir *= -1;
      }
    }

    sinDir = dir;
    sinLast = sinCrr;
    return 50;
}


/**
 * 全部
 */
int all() {
    for(int i = 0; i < PIN_LEN; i++) {
      lines[i] = 1;
    }
    return 100;
}


/**
 * 交互
 */
int toggleCnt = 0;
int toggle() {
  for(int i = 0; i < PIN_LEN; i++) {
    if(
      toggleCnt % 2 == 0 && i % 2 == 0 ||
      toggleCnt % 2 == 1 && i % 2 == 1
    ) {
      lines[i] = 1;
    }
  }
  toggleCnt++;
  return 100;
}
