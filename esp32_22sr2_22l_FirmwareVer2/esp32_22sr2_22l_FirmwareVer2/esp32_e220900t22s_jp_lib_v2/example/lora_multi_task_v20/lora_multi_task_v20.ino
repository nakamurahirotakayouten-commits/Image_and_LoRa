#include "esp32_e220900t22s_jp_lib_v2.h"
#include <Arduino.h>
#include <FS.h>
#include <SD.h>
#include <SPI.h>

#define CHECK_STRICT_EXECUTION 0       //  Checking Strict Mode execution

int ReceiveComplete;

CLoRa lora;

/** prototype declaration **/
void LoRaRecvTask(void *pvParameters);
void LoRaSendTask(void *pvParameters);
void ReadDataFromConsole(char *msg, int max_msg_len);
void DispReceivedData();


void setup() {
  // put your setup code here, to run once:
  SerialMon.begin(9600);
  delay(1000);          // SerialMon init wait
  SerialMon.println();
  SerialLoRa.end();     // end()を実行　←←追加
  delay( 800 );         // 800m秒待つ　 ←←追加
  SerialLoRa.begin(9600, SERIAL_8N1, 16, 17);

  //  pinMode(ledPin, OUTPUT);
  // コンフィグモード(M0=1,M1=1)へ移行する
  lora.SwitchToConfigurationMode();
  
  // .ini ファイルを使用しない場合
//  lora.SetDefaultConfigValue(lora.config);       // ライブラリ内のコンフィグデフォルト値設定

  // .ini から LoRa設定値を読み込む
  if (lora.LoadConfigSetting(CONFIG_FILENAME, lora.config)) {
    SerialMon.printf("\nLoading Configfile failed. The default value is set.\n");
  } else {
    SerialMon.printf("\nLoading Configfile succeeded.\n");
  }

  lora.init_REG09_0A_Byte();

  // E220-900T22S(JP)へのLoRa初期設定
  if (lora.InitLoRaModule(lora.config)) {
    SerialMon.printf("init error\n");
    return;
  }

// this sample must execute in strict mode, so forced set enable REG09 bit to strict mode
//  lora.conf_REG09_Byte |= StrictMode_EN;
//  lora.StrictModeStart();

  if (lora.SetConfigStrictMode()) {
    SerialMon.printf("Strict mode init error\n");
    return;
  }
  
  // コンフィグモード(M0=1,M1=1)へ移行する
  lora.SwitchToConfigurationMode();

  // Strict Mode　個体番号レジスタの書込み確認
  lora.write_id_num(0xE0, 0x55);
  if ( !lora.read_id_num(0xE0) ) {
    uint8_t num = lora.REGData.REGUniStr.val[0];
    SerialMon.printf("\nIDnum Read after write : %02x \n", num);
  } else {
    SerialMon.printf("\nIDnum Read after write error.\n");
  }
  SerialMon.printf("\nconf_REG09_Byte = %02x\n",lora.conf_REG09_Byte);

#if CHECK_STRICT_EXECUTION == 1    //  Checking Strict Mode execution
  lora.init_REG09_0A_Byte();

  lora.REG09_Byte = StrictMode_EN;
  SerialMon.printf("\nInput 0, 1, 2, 3, 4 : \n");
  
  while (SerialMon.available() == 0);
  
  char incoming_byte = SerialMon.read();
  while (SerialMon.available()) SerialMon.read();

  switch (incoming_byte) {
    case '0' :
      SerialMon.printf("........ No Specified.\n");
      break;
    case '1' :
      SerialMon.printf("........ Add Packet Size.\n");
      lora.ReceivePKTaddSIZE();
      break;
    case '2' :
      SerialMon.printf("........ Add Packet Size.\n");
      SerialMon.printf("........ Add Send Packet CheckSum.\n");
      lora.ReceivePKTaddSIZE();
      lora.SendPKTaddCS();
      break;
    case '3' :
      SerialMon.printf("........ Add Packet Size.\n");
      SerialMon.printf("........ Add Send Packet CheckSum.\n");
      SerialMon.printf("........ Add Receive Packet CheckSum.\n");
      lora.ReceivePKTaddSIZE();
      lora.SendPKTaddCS();
      lora.ReceivePKTaddCS();
      break;
    case '4' :
      SerialMon.printf("........ Add Packet Size.\n");
      SerialMon.printf("........ Add Send Packet CheckSum.\n");
      SerialMon.printf("........ Add Receive Packet CheckSum.\n");
      SerialMon.printf("........ Add Received Target Address.\n");
      lora.ReceivePKTaddSIZE();
      lora.SendPKTaddCS();
      lora.ReceivePKTaddCS();
      lora.ReceivePKTaddADRS();
      break;
    default: break;
  }
  while (SerialMon.available() > 0) {
    SerialMon.read();
    delay(1);
  }
#endif

  // ノーマルモード(M0=0,M1=0)へ移行する
  lora.SwitchToNormalMode();
  
  ReceiveComplete = 0;

  //  // マルチタスク
  xTaskCreateUniversal(LoRaRecvTask, "LoRaRecvTask", 8192, NULL, 1, NULL,
                       APP_CPU_NUM);
  xTaskCreateUniversal(LoRaSendTask, "LoRaSendTask", 8192, NULL, 1, NULL,
                       APP_CPU_NUM);
//  lora.SwitchToWORSendingMode();

}
void loop() {

  // put your main code here, to run repeatedly:
  delay(1000);

}

void LoRaRecvTask(void *pvParameters) {
  
  while (1) {
    while (ReceiveComplete) {
      delay(10);                   // be suitable
    }

//    delay(10000);                  // for storing Multi-Packet and checking

    if (lora.ReceiveFrame(&lora.data) == 0) {
      ReceiveComplete = 1;

      SerialMon.printf("recv data:\n");
      //      for (int i = 0; i < lora.data.recv_data_len; i++) {
      //        SerialMon.printf("%c", lora.data.recv_data[i]);
      //      }
      //      SerialMon.printf("\n");
      //      SerialMon.printf("hex dump:\n");
      for (int i = 0; i < lora.data.recv_data_len; i++) {
        SerialMon.printf("%02x ", lora.data.recv_data[i]);
        if ( i > 32 ) break;
      }
      SerialMon.printf("\n");
      SerialMon.printf("RSSI: %d dBm\n", lora.data.rssi);
      SerialMon.printf("\n");

      SerialMon.flush();
    }
    DispReceivedData();

    ReceiveComplete = 0;

    delay(1);
  }
}

void LoRaSendTask(void *pvParameters) {

  int modef = 0;

  while (1) {
    char msg[200] = {0};

    // ESP32がコンソールから読み込む
    ReadDataFromConsole(msg, (sizeof(msg) / sizeof(msg[0])));


    if (lora.SendFrame(lora.config, (uint8_t *)msg, strlen(msg)) == 0) {
      SerialMon.printf("\nsend succeeded.\n");
      SerialMon.printf("\n");
    } else {
      SerialMon.printf("\nsend failed.\n");
      SerialMon.printf("\n");
    }

    SerialMon.flush();
    delay(1);

  }
}

void DispReceivedData() {
  for (int j = 0;; j++) {

    SerialMon.printf("\n RcvPacket[%d].packet_size = %d", j, lora.RcvPacket[j].packet_size);
    SerialMon.printf("\n RcvPacket[%d].packet_address = %02x%02x", j, lora.RcvPacket[j].packet_address >> 8 , lora.RcvPacket[j].packet_address & 0xff);
    SerialMon.printf("\n RcvPacket[%d].packet_data = ", j);
    for (int i = 0; i < lora.RcvPacket[j].packet_size; i++) {
      SerialMon.printf(" %02x", lora.RcvPacket[j].packet_data[i]);
      if ( i > 32 ) break;
    }
    SerialMon.printf("\n RcvPacket[%d].recv_multi_pkt_f = %d", j, lora.RcvPacket[j].recv_multi_pkt_f);
    SerialMon.printf("\n RcvPacket[%d].packet_rssi = %d\n", j, lora.RcvPacket[j].packet_rssi);

    if (lora.RcvPacket[j].recv_multi_pkt_f == 0) break;

  }
  SerialMon.flush();
  delay(100);

}


void ReadDataFromConsole(char *msg, int max_msg_len) {
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
