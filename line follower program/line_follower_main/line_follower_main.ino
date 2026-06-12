int TH0 = 400, TH1 = 210, TH2 = 200, TH3 = 200, TH4 = 400, TH5 = 400, TH6 = 210, TH7 = 210; //しきい値
int leftPIN_F  = 4;   //左モータ 正転
int leftPIN_B  = 5;   //左モータ 逆転
int rightPIN_F = 8;  //右モータ 正転
int rightPIN_B = 9;   //右モータ 逆転
int speedValue = 255;
int leftlast   = -1;  //左側の最終入力
int rightlast  = -1;  //右側の最終入力
int kousa      = -1;  //交差点を通った回数
int i = 0;            //終端の交差点を通った回数
bool lock = false;    //黒線かどうか
float LD;             //左デューティ比
float RD;             //右デューティ比

void setup() {
  pinMode(13, OUTPUT);
  
  pinMode(leftPIN_F, OUTPUT);
  pinMode(leftPIN_B, OUTPUT);
  pinMode(rightPIN_F, OUTPUT);
  pinMode(rightPIN_B, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  int sensorValueA0 = analogRead(A12); //右1 中央
  int sensorValueA1 = analogRead(A13); //右2
  int sensorValueA2 = analogRead(A14); //右3
  int sensorValueA3 = analogRead(A15); //右4 端
  int sensorValueA4 = analogRead(A11); //左1 中央
  int sensorValueA5 = analogRead(A10); //左2
  int sensorValueA6 = analogRead(A9); //左3
  int sensorValueA7 = analogRead(A8); //左4 端
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

  if (kousa == 4){ //四つ目の交差点の時
    i++;
    
    if (i % 2 == 0){
      //その場で180°回転
      analogWrite(leftPIN_F, speedValue * 0.3);
      analogWrite(leftPIN_B, 0);
      analogWrite(rightPIN_F, 0);
      analogWrite(rightPIN_B, speedValue * 0.3);
      delay (300);

      while (analogRead(A7) < TH7){
        analogWrite(leftPIN_F, speedValue * 0.3);
        analogWrite(leftPIN_B, 0);
        analogWrite(rightPIN_F, 0);
        analogWrite(rightPIN_B, speedValue * 0.3);
      }
      kousa = 0;
    }
    else {
      //ライントレースを抜けてボール回収
      kousa = 0;
    }
  }
  else{
    //黒線を踏んでる場合
    if (TH0 < sensorValueA0 || TH1 < sensorValueA1 || TH2 < sensorValueA2 || TH3 < sensorValueA3 || TH4 < sensorValueA4 || TH5 < sensorValueA5 || TH6 < sensorValueA6 || TH7 < sensorValueA7){
      //---交差点---//
      if(TH1 < sensorValueA1 && TH5 < sensorValueA5){
        //ロック(複数回読み取り防止)
        if (lock == false){
          kousa++;
          lock = true;
          digitalWrite(13, HIGH);
        }
      }
      
      if (TH0 < sensorValueA0){
        LD = 0.2;
        RD = -0.1;
      }
      else if (TH1 < sensorValueA1){
        LD = 0.3;
        RD = -0.25;
      }
      else if (TH2 < sensorValueA2){
        LD = 0.2;
        RD = -0.2;
      }
      else if (TH3 < sensorValueA3){
        LD = 0.15;
        RD = -0.3;
      }
      else if (TH4 < sensorValueA4){
        LD = -0.1;
        RD = 0.2;
      }
      else if (TH5 < sensorValueA5){
        LD = -0.25;
        RD = 0.3;
      }
      else if (TH6 < sensorValueA6){
        LD = -0.2;
        RD = 0.2;
      }
      else if (TH7 < sensorValueA7){
        LD = -0.3;
        RD = 0.15;
      }
      //---左モータ---//
      /*if (TH < sensorValueA0 && TH < sensorValueA1 && TH < sensorValueA2 && TH < sensorValueA3){
        LD = 0.9; //右1と右2と右3と右4がON：出力90%
        rightlast = 3;
      }
      else if (TH < sensorValueA0 && TH < sensorValueA1 && TH < sensorValueA2){
        LD = 0.75; //右1と右2と右3がON：出力70%
        rightlast = 2;
      }
      else if (TH < sensorValueA0 && TH < sensorValueA1){
        LD = 0.6; //右1と右2がON：出力50%
        rightlast = 1;
      }
      else if ( TH < sensorValueA3){
        LD = 0.9; //右4のみON：出力90%
        rightlast = 3;
      }
      else if ( TH < sensorValueA2){
        LD = 0.75; //右3のみON：出力70%
        rightlast = 2;
      }
      else if (TH < sensorValueA1){
        LD = 0.6; //右2のみON：出力50%
        rightlast = 1;
      }
      else if (TH < sensorValueA0){
        LD = 0.3; //右1のみON：出力30%
        rightlast = 0;
      }
      else{
        LD = 0.15;
      }

      //---右モータ---//
      if (TH < sensorValueA4 && TH < sensorValueA5 && TH < sensorValueA6 && TH < sensorValueA7){
        RD = 0.9; //左1と2と左3と左4がON：出力90%
        leftlast = 3;
      }
      else if (TH < sensorValueA4 && TH < sensorValueA5 && TH < sensorValueA6){
        RD = 0.75; //左1と左2と左3がON：出力70%
        leftlast = 2;
      } 
      else if (TH < sensorValueA4 && TH < sensorValueA5){
        RD = 0.6; //左1と左2がON：出力50%
        leftlast = 1;
      }
      else if (TH < sensorValueA7){
        RD = 0.9; //左4のみON：出力90%
        leftlast = 3;
      }
      else if (TH < sensorValueA6){
        RD = 0.75; //左3のみON：出力70%
        leftlast = 2;
      }
      else if (TH < sensorValueA5){
        RD = 0.6; //左2のみON：出力50%
        leftlast = 1;
      }
      else if (TH < sensorValueA4){
        RD = 0.3; //左1のみON：出力30%
        leftlast = 0;
      }
      else{
        RD = 0.15;
      }*/
    }
    //黒線を踏んでない場合
    else{
      //ロック解除
      lock = false;
      digitalWrite(13, LOW);

      //---探索---//
      if (leftlast == 0 || rightlast == 0){
        LD = -0.3; //最後の入力が中央：左出力-30% 右出力40％
        RD = 0.4;
      }
      else if (rightlast == 1 || rightlast == 2 || rightlast == 3){
        LD = 0.5;//最後の入力が右側：左出力50％
        RD = 0.0; 
      }
      else if (leftlast == 1 || leftlast == 2 || leftlast == 3){
        LD = 0.0;//最後の入力が左側：右出力50%
        RD = 0.5; 
      }
      else{
        LD = 0.3;
        RD = 0.3;
      }
    }
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
