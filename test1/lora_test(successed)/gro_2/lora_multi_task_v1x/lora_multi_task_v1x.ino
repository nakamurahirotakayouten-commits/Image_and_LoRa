#include "esp32_e220900t22s_jp_lib_v2.h"
#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

// this is gro_2!!!!



//const int ledPin = 2;

int ReceiveComplete;
int counter = 0;

CLoRa lora;
//struct LoRaConfigItem_t config;
//struct RecvFrameE220900T22SJP_t data;

/** prototype declaration **/
void LoRaRecvTask(void *pvParameters);
void LoRaSendTask(void *pvParameters);
void ReadDataFromConsole(char *msg, int max_msg_len);

void setup() {
  // put your setup code here, to run once:
  SerialMon.begin(9600);
  delay(1000); // SerialMon init wait
  SerialMon.println();

  //  pinMode(ledPin, OUTPUT);



  // LoRa設定値の読み込み
  if (lora.LoadConfigSetting(CONFIG_FILENAME, lora.config)) {
    SerialMon.printf("Loading Configfile failed. The default value is set.\n");
  } else {
    SerialMon.printf("Loading Configfile succeeded.\n");
  }

  // E220-900T22S(JP)へのLoRa初期設定
  if (lora.InitLoRaModule(lora.config)) {
    SerialMon.printf("init error\n");
    for (;;) {
      //
      //      digitalWrite(ledPin,HIGH);
      //      delay(1000);
      //      digitalWrite(ledPin,LOW);
    }
    return;
  }

  // SerialMon.print("AUX end = ");
  // SerialMon.println(digitalRead(LoRa_AUXPin)); //でも成功しても AUXはLOWでした．

  // ノーマルモード(M0=0,M1=0)へ移行する
  lora.SwitchToNormalMode();

  ReceiveComplete = 0;

  // マルチタスク
  xTaskCreateUniversal(LoRaRecvTask, "LoRaRecvTask", 8192, NULL, 1, NULL,
                       APP_CPU_NUM);
  xTaskCreateUniversal(LoRaSendTask, "LoRaSendTask", 8192, NULL, 1, NULL,
                       APP_CPU_NUM);
                    
  // LoRaRecvTask(); //ループさせます．<- だめでした．プログラムとしてダメ．
  SerialMon.print("setup done.");
  
}

void loop() {

  // put your main code here, to run repeatedly:
  delay(1000);

  
}

void LoRaRecvTask(void *pvParameters) {

  while (1) {

    // SerialMon.printf("\rnow searching : %0002d",counter); //この関数が回っているのかの確認
    // counter += 1;
    // if(counter > 1000){
      // counter = 0;
    // } // やっぱりループしていない．

    if (ReceiveComplete) {
      delay(10);                   // be suitable
      continue;
    }

    if (lora.ReceiveFrame(&lora.data) == 0) { //ここが受信まで待っていてくれる．

      SerialMon.printf("\nrecv data:\n");

     for (int i = 0; i < lora.data.recv_data_len; i++) { //データ部分
       SerialMon.printf("%c", lora.data.recv_data[i]);
     }
     SerialMon.printf("\n");

      SerialMon.printf("hex dump:\n");
      for (int i = 0; i < lora.data.recv_data_len; i++) {
        SerialMon.printf("%02x ", lora.data.recv_data[i]); //データを受け取って表示している．一番中心的なコードその１
      }
      SerialMon.printf("\n");
      SerialMon.printf("RSSI: %d dBm\n", lora.data.rssi);
      SerialMon.printf("\n");

      SerialMon.flush();
    }

    ReceiveComplete = 0;

    delay(1);
  }
}

void LoRaSendTask(void *pvParameters) { //esp32から受け取ったデータをアンテナから送る関数
  while (1) {
    char msg[200] = {0};

    // ESP32がコンソールから読み込む
    ReadDataFromConsole(msg, (sizeof(msg) / sizeof(msg[0])));

    if (lora.SendFrame(lora.config, (uint8_t *)msg, strlen(msg)) == 0) { //普通にこのifを流すだけで送信できる．すばらしいね．一番中心的なコードその２
      SerialMon.printf("send succeeded.\n");
      SerialMon.printf("\n");
    } else {
      SerialMon.printf("send failed.\n");
      SerialMon.printf("\n");
    }

    SerialMon.flush();

    delay(1);
  }
}

void ReadDataFromConsole(char *msg, int max_msg_len) { //Loraがesp32からデータを受け取る関数
  int len = 0;
  char *start_p = msg;

  while (len < max_msg_len) {
    if (SerialMon.available() > 0) {
      char incoming_byte = SerialMon.read();
      if (incoming_byte == 0x00 || incoming_byte > 0x7F)
        continue;
      *(start_p + len) = incoming_byte;
      // 最短で3文字(1文字 + CR LF)
      if (incoming_byte == 0x0a && len >= 2 && (*(start_p + len - 1)) == 0x0d) {
        break;
      }
      len++;
    }
    delay(1);
  }

  // msgからCR LFを削除
  len = strlen(msg);
  for (int i = 0; i < len; i++) {
    if (msg[i] == 0x0D || msg[i] == 0x0A) {
      msg[i] = '\0';
    }
  }
}
