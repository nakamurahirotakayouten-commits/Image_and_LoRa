HardwareSerial MySerial(1);

void setup() {
  Serial.begin(115200);

  MySerial.begin(115200, SERIAL_8N1, 16, 17);

  delay(1000);
  
  
}

uint8_t data[] = {0xAA, 0xBB, 0xCC, 0xDD};
uint8_t temp[] = {0x00};
uint8_t rdata[200] = {0x00};

// int rsize = 0;
int size = 0;
int rec = 0;

void loop() {
  
  if(Serial.available()){ //もしpcからシリアルでなんか文字が来たら，

    if(Serial.read() == '1'){ //来たのが 1 なら，
      MySerial.write(data, sizeof(data)); //バイナリデータを送る．
      Serial.print("bin sended\n");  //バイナリ送ったよ．
      while (Serial.available()) {
        Serial.read();
      }
    }
    else{
      Serial.print("バイナリを送る場合は 1 と書いてね\n");
      while (Serial.available()) {
        Serial.read();
      } 
    }
    // Serial.println("your message received");
    // String rec_pc = Serial.readStringUntil('\n');
    // MySerial.println(rec_pc);
    // Serial.println("your message sended");
  }
  
  delay(100);

//受信フェーズ

  if (MySerial.available() > 1) {

    // Serial.println("じ");

    size = MySerial.available();
    delay(50);
    size = MySerial.readBytes((char*)temp, size);

    // tempの内容を表示
    for (int i = 0; i < size; i++) {Serial.printf("%02X ", temp[i]);}

    Serial.println();

    // rdataの末尾へ追加
    Serial.println(size);

    for (int i = 0; i < size && rec < sizeof(rdata); i++) {
        rdata[rec] = temp[i];
        // Serial.println(rec);
        rec = rec + 1;
        // Serial.print("ここ");
    }

    // rdata全体を表示
    for (int i = 0; i < rec; i++) {
        Serial.printf("%02X ", rdata[i]);
    }
    Serial.println();
    Serial.println(rec);
    Serial.println();
  }
  
}