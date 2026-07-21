HardwareSerial MySerial(1);

void setup() {
  Serial.begin(115200);

  MySerial.begin(115200, SERIAL_8N1, 16, 17);

  delay(1000);
  
}

void loop() {
  
  if(Serial.available()){
    // Serial.println("your message received");
    String rec_pc = Serial.readStringUntil('\n');
    MySerial.println(rec_pc);
    Serial.println("your message sended");
  }
  
  delay(100);

  if(MySerial.available()) {
    String c = MySerial.readStringUntil('\n');
    Serial.println(c);
    c = "";
  }
}