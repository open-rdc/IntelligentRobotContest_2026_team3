#include <Servo.h>

int TH[]       = {400, 210, 200, 200, 400, 400, 210, 210}; //しきい値
int kTH        = 400;   //距離センサのしきい値
int KS         = 0;     //距離センサの読み取り回数
int leftPIN_F  = 4;     //左モータ 正転
int leftPIN_B  = 5;     //左モータ 逆転
int rightPIN_F = 8;     //右モータ 正転
int rightPIN_B = 9;     //右モータ 逆転
int speedValue = 255;
int last       = -1;    //最終入力
int kousa      = -1;    //交差点を通った回数
int i          = 0;     //終端の交差点を通った回数
int mode       = 0;
int sensorValueA0, sensorValueA1, sensorValueA2, sensorValueA3, sensorValueA4, sensorValueA5, sensorValueA6, sensorValueA7;
int roller;
int servoPin = 7;       //信号線を繋いだピン番号
bool lock      = false; //黒線かどうか
float LD;               //左デューティ比
float RD;               //右デューティ比
unsigned long startTime = millis(); // 走り始めの時間を記録

Servo myServo;

void runTrace() {
  if (TH[1] < sensorValueA1 && TH[5] < sensorValueA5){
    //ロック(複数回読み取り防止)
    if (lock == false){
      kousa++;
      lock = true;
      digitalWrite(13, HIGH);
    }
    LD = 0.3;
    RD = 0.3;
  }
  else if (TH[0] < sensorValueA0){
    LD = 0.25;
    RD = -0.1;
    last = 0;
  }
  else if (TH[1] < sensorValueA1){
    LD = 0.4;
    RD = -0.35;
    last = 1;
  }
  else if (TH[2] < sensorValueA2){
    LD = 0.3;
    RD = -0.3;
    last = 1;
  }
  else if (TH[3] < sensorValueA3){
    LD = 0.25;
    RD = -0.4;
    last = 1;
  }
  else if (TH[4] < sensorValueA4){
    LD = -0.1;
    RD = 0.25;
    last = 0;
  }
  else if (TH[5] < sensorValueA5){
    LD = -0.35;
    RD = 0.4;
    last = 2;
  }
  else if (TH[6] < sensorValueA6){
    LD = -0.3;
    RD = 0.3;
    last = 2;
  }
  else if (TH[7] < sensorValueA7){
    LD = -0.4;
    RD = 0.25;
    last = 2;
  }
  else if (TH[1] > sensorValueA1 && TH[5] > sensorValueA5){
    //ロック解除
    lock = false;
    digitalWrite(13, LOW);
  }
}

void leftTurn(){
  unsigned long startTime = millis();
  unsigned long currentTime;

  do{
    unsigned long currentTime = millis() - startTime;
    analogWrite(leftPIN_F, 0);
    analogWrite(leftPIN_B, speedValue * 0.3);
    analogWrite(rightPIN_F, speedValue * 0.3);
    analogWrite(rightPIN_B, 0);
  }while(currentTime > 0 && currentTime < 3000);
}

void rightTurn(){
  unsigned long startTime = millis();
  unsigned long currentTime;

  do{
    unsigned long currentTime = millis() - startTime;
    analogWrite(leftPIN_F, speedValue * 0.3);
    analogWrite(leftPIN_B, 0);
    analogWrite(rightPIN_F, 0);
    analogWrite(rightPIN_B, speedValue * 0.3);
  }while(currentTime > 0 && currentTime < 3000);
}

void runTurn() {
    //その場で180°回転
    analogWrite(leftPIN_F, speedValue * 0.3);
    analogWrite(leftPIN_B, 0);
    analogWrite(rightPIN_F, 0);
    analogWrite(rightPIN_B, speedValue * 0.3);
    delay (300);

    while (analogRead(A7) < TH[7]){
      analogWrite(leftPIN_F, speedValue * 0.3);
      analogWrite(leftPIN_B, 0);
      analogWrite(rightPIN_F, 0);
      analogWrite(rightPIN_B, speedValue * 0.3);
    }
    kousa = 0;
}

void runSearch() {
  if (last == 0){
    LD = -0.3; //最後の入力が中央：左出力-30% 右出力40％
    RD = 0.4;
  }
  else if (last == 1){
    LD = 0.5;//最後の入力が右側：左出力50％
    RD = 0.0; 
  }
  else if (last == 2){
    LD = 0.0;//最後の入力が左側：右出力50%
    RD = 0.5; 
  }
  else{
    LD = 0.3;
    RD = 0.3;
  }
}

void ballSearch() {
  unsigned long currentTime;
  analogWrite(roller, 255);

  unsigned long startTime = millis();
  do{
    unsigned long currentTime = millis() - startTime;
    rightTurn();
  }while(currentTime > 0 && currentTime < 3000);

  startTime = millis();
  do{
    unsigned long currentTime = millis() - startTime;
    analogWrite(leftPIN_F, speedValue * 0.3);
    analogWrite(leftPIN_B, 0);
    analogWrite(rightPIN_F, speedValue * 0.3);
    analogWrite(rightPIN_B, 0);
  }while(currentTime > 0 && currentTime < 1000);

  startTime = millis();
  do{
    unsigned long currentTime = millis() - startTime;
    leftTurn();
  }while(currentTime > 0 && currentTime < 3000);

  startTime = millis();
  do{
    unsigned long currentTime = millis() - startTime;
    analogWrite(leftPIN_F, speedValue * 0.3);
    analogWrite(leftPIN_B, 0);
    analogWrite(rightPIN_F, speedValue * 0.3);
    analogWrite(rightPIN_B, 0);
  }while(currentTime > 0 && currentTime < 5000);

  startTime = millis();
  do{
    unsigned long currentTime = millis() - startTime;
    leftTurn();
  }while(currentTime > 0 && currentTime < 3000);

  startTime = millis();
  do{
    unsigned long currentTime = millis() - startTime;
    analogWrite(leftPIN_F, speedValue * 0.3);
    analogWrite(leftPIN_B, 0);
    analogWrite(rightPIN_F, speedValue * 0.3);
    analogWrite(rightPIN_B, 0);
  }while(currentTime > 0 && currentTime < 3000);

  startTime = millis();
  do{
    unsigned long currentTime = millis() - startTime;
    leftTurn();
  }while(currentTime > 0 && currentTime < 3000);

  startTime = millis();
  do{
    unsigned long currentTime = millis() - startTime;
    analogWrite(leftPIN_F, speedValue * 0.3);
    analogWrite(leftPIN_B, 0);
    analogWrite(rightPIN_F, speedValue * 0.3);
    analogWrite(rightPIN_B, 0);
  }while(currentTime > 0 && currentTime < 5000);

  startTime = millis();
  do{
    unsigned long currentTime = millis() - startTime;
    leftTurn();
  }while(currentTime > 0 && currentTime < 3000);

  startTime = millis();
  do{
    unsigned long currentTime = millis() - startTime;
    analogWrite(leftPIN_F, speedValue * 0.3);
    analogWrite(leftPIN_B, 0);
    analogWrite(rightPIN_F, speedValue * 0.3);
    analogWrite(rightPIN_B, 0);
  }while(currentTime > 0 && currentTime < 1000);

  startTime = millis();
  do{
    unsigned long currentTime = millis() - startTime;
    rightTurn();
  }while(currentTime > 0 && currentTime < 3000);
}

void servo() {
  // 0度へ移動
  myServo.write(0);
  delay(1000); // 1秒待機

  // 90度へ移動
  myServo.write(50);
  delay(1000); // 1秒待機
}

void haisyutu(){
  if (KS == -1){
    if (/*カラーセンサが赤*/){
      servo();
    }
    else if (/*カラーセンサが黄色*/){
      while (true){
        LD = -0.3;
        RD = -0.3;

        if (KS == 1){
          break;
        }
      }
      servo();
    }
    else if (/*カラーセンサが青*/){
      while (true){
        LD = -0.3;
        RD = -0.3;

        if (KS == 3){
          break;
        }
      }
      servo();
    }
  }
  else if (KS == 0){
    if (/*カラーセンサが赤*/){
      while (true){
        LD = 0.3;
        RD = 0.3;

        if (KS == -1){
          break;
        }
      }
      servo();      
    }
    else if (/*カラーセンサが黄色*/){
      while (true){
        LD = -0.3;
        RD = -0.3;

        if (KS == 1){
          break;
        }
      }
      servo();
    }
    else if (/*カラーセンサが青*/){
      while (true){
        LD = -0.3;
        RD = -0.3;

        if (KS == 3){
          break;
        }
      }
      servo();
    }
  }
  else if (KS == 1){
    if (/*カラーセンサが赤*/){
      while (true){
        LD = 0.3;
        RD = 0.3;

        if (KS == -1){
          break;
        }
      }
      servo();      
    }
    else if (/*カラーセンサが黄色*/){
      servo();
    }
    else if (/*カラーセンサが青*/){
      while (true){
        LD = -0.3;
        RD = -0.3;

        if (KS == 3){
          break;
        }
      }
      servo();
    }
  }
  else if (KS == 3){
    if (/*カラーセンサが赤*/){
      while (true){
        LD = 0.3;
        RD = 0.3;

        if (KS == -1){
          break;
        }
      }
      servo();       
    }
    else if (/*カラーセンサが黄色*/){
      while (true){
        LD = 0.3;
        RD = 0.3;

        if (KS == 1){
          break;
        }
      }
      servo();
    }
    else if (/*カラーセンサが青*/){
      servo();
    }
  }
}

void setup() {
  pinMode(13, OUTPUT);
  pinMode(leftPIN_F, OUTPUT);
  pinMode(leftPIN_B, OUTPUT);
  pinMode(rightPIN_F, OUTPUT);
  pinMode(rightPIN_B, OUTPUT);

  myServo.attach(servoPin, 500, 2500);
  Serial.begin(9600);
}

void loop() {
  sensorValueA0 = analogRead(A12); //右1 中央
  sensorValueA1 = analogRead(A13); //右2
  sensorValueA2 = analogRead(A14); //右3
  sensorValueA3 = analogRead(A15); //右4 端
  sensorValueA4 = analogRead(A11); //左1 中央
  sensorValueA5 = analogRead(A10); //左2
  sensorValueA6 = analogRead(A9); //左3
  sensorValueA7 = analogRead(A8); //左4 端
  Serial.print(sensorValueA0);
  Serial.print(", ");
  Serial.print(sensorValueA1);
  Serial.print(", ");
  Serial.print(sensorValueA2);
  Serial.print(", ");
  Serial.print(sensorValueA3);
  Serial.print(", ");
  Serial.print(sensorValueA4);
  Serial.print(", ");
  Serial.print(sensorValueA5);
  Serial.print(", ");
  Serial.print(sensorValueA6);
  Serial.print(", ");
  Serial.println(sensorValueA7);

  if (LD > 0 && /*距離センサ*/ > kTH){
    KS--;
  }
  else if (LD < 0 && /*距離センサ*/ > kTH){
    KS++;
  }

  if (kousa == 4){
    i++;
    if (i / 2 == 0){
      KS = 0;
      while(true){
        if (/*カラーセンサがon*/){
          mode = 4;
        }
        else{
          kousa = 1;
          break;
        }
      }
      mode = 1; // 4回目の交差点だから旋回モードへ
    }
    else{
      mode = 3; // ボール探索モード
    }
  } 
  else if (sensorValueA0 < TH[0] || sensorValueA1 < TH[1] || sensorValueA2 < TH[2] || sensorValueA3 < TH[3] || sensorValueA4 < TH[4] || sensorValueA5 < TH[6] || sensorValueA7 < TH[7] ) {
    mode = 2; // ライン探索モードへ
  } 
  else {
    mode = 0; // 通常トレースモードへ
  }

  switch (mode) {
    case 0: // トレース中の動き
      runTrace();
      break;
    case 1: // 旋回中の動き
      runTurn();
      break;
    case 2: // ライン探索中の動き
      runSearch();
      break;
    case 3: // ボール探索中の動き
      ballSearch();
      break;
    case 4: // 排出時の動き
      haisyutu();
      break;
  }

  //---モータ出力---//
  if (LD >= 0){ //左正転
    analogWrite(leftPIN_F, speedValue * LD);
    analogWrite(leftPIN_B, 0);
  }
  else{         //左逆転
    analogWrite(leftPIN_F, 0);
    analogWrite(leftPIN_B, speedValue * (LD*-1));
  }

  if (RD >= 0){ //右正転
    analogWrite(rightPIN_F, speedValue * RD);
    analogWrite(rightPIN_B, 0);
  }
  else{         //右逆転
    analogWrite(rightPIN_F, 0);
    analogWrite(rightPIN_B, speedValue * (RD*-1));
  }

  /*Serial.print(LD);
  Serial.print(", ");
  Serial.println(RD);*/
}
