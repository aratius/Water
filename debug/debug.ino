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
  _delay = zigzag();
  // _delay = sinWave();
  // _delay = all();
  // _delay = toggle();
  // _delay = lissajous();
  // _delay = curtain();
  
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
  Serial.print("||| ");
  for(int i = 0;i < PIN_LEN; i++) {
    if(lines[i] == 0) Serial.print(" ");
    else if(lines[i] == 1) Serial.print("■");
    else Serial.print("◆");
    Serial.print(" ");
  }
  Serial.print("|||");
  Serial.println();
}


/**
 * ステータスを比べて変化があればdigitalzWriteする
 */
void compareLines() {
  for(int i = 0; i < PIN_LEN; i++) {
    int crr = lines[i];
    int last = beforeLines[i];

    // 状態変化があれば弁の開閉を行う
    // 1はかってに閉じるので、閉は2のときだけ実行する
    if(
      last == 0 && (crr == 1 || crr == 2) ||
      last == 2 && crr == 0
    ) {
      doValve(i);
    }
    
  }
  
}


/**
 * 状態更新
 */
void updateStatus() {
  for(int i = 0; i < PIN_LEN; i++) {
    beforeLines[i] = lines[i];
  }
}


/**
 * 弁の開閉
 * OFF時間のスケジューリング
 */
void doValve(int i) {
  digitalWrite(pins[i], lines[i]);
  if(lines[i] == 1) openedTime[i] = (float)millis();
}


/**
 * OFFにすべき時間を過ぎたら弁を閉じる
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


/**
 * リサージュ曲線っぽいやつ
 */
int lissajousDir = 1;
float lissajousLast[] = {0, 0};
int lissajous() {
  int dir = lissajousDir;
  bool shouldToggleDir = false;
  unsigned long ms = millis();
  float t = (float)ms * 0.006;

  for(int j = 0; j < 2; j++) {
    int dirEach = (j % 2) * 2 - 1;
    float lissajousCrr = ((sin(t) * (float)dirEach) + 1) * 0.5 * (float)PIN_LEN;
    for(int i = 0; i < PIN_LEN; i++) {
      float pos = (float)i + 0.5;
      if(
        dir == 1 * (float)dirEach && (pos > lissajousLast[j] && pos <= lissajousCrr) ||
        dir == -1 * (float)dirEach && (pos < lissajousLast[j] && pos >= lissajousCrr)
      ) {
        lines[i] = 1;
        if(
          (i == 0 && dir * (float)dirEach== -1) || 
          (i == PIN_LEN - 1 && dir * (float)dirEach == 1)
        ) shouldToggleDir = true;
      }
    }
    lissajousLast[j] = lissajousCrr;
  }

  if(shouldToggleDir) lissajousDir *= -1;
  return 50;
}


/**
 * カーテン
 */
int curtain() {
  for(int i = 0; i < PIN_LEN; i++) {
    if(i % 2 == 0 && lines[i] == 0) {
      lines[i] = 2;
    } else if (i % 2 == 1) {
      lines[i] = 1;
    }
  }
  return 100;
}
