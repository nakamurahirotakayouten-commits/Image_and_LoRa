#include "Arduino.h"
#define CAMERA_MODEL_ESP32S3_EYE
#include "sd_read_write.h"
#include "SD_MMC.h"
#include "ws2812.h"
#include "esp_camera.h"
#include "camera_index.h"
#include "camera_pins.h"

#define BUTTON_PIN  0
camera_config_t config;


String serialinput = "";
// bool stringComplete = false;
bool shot = false;
int counter = 1;


int cameraSetup(void) {
//   config.ledc_channel = LEDC_CHANNEL_0;
//   config.ledc_timer = LEDC_TIMER_0;
//   config.pin_d0 = Y2_GPIO_NUM;
//   config.pin_d1 = Y3_GPIO_NUM;
//   config.pin_d2 = Y4_GPIO_NUM;
//   config.pin_d3 = Y5_GPIO_NUM;
//   config.pin_d4 = Y6_GPIO_NUM;
//   config.pin_d5 = Y7_GPIO_NUM;
//   config.pin_d6 = Y8_GPIO_NUM;
//   config.pin_d7 = Y9_GPIO_NUM;
//   config.pin_xclk = XCLK_GPIO_NUM;
//   config.pin_pclk = PCLK_GPIO_NUM;
//   config.pin_vsync = VSYNC_GPIO_NUM;
//   config.pin_href = HREF_GPIO_NUM;
//   config.pin_sccb_sda = SIOD_GPIO_NUM;
//   config.pin_sccb_scl = SIOC_GPIO_NUM;
//   config.pin_pwdn = PWDN_GPIO_NUM;
//   config.pin_reset = RESET_GPIO_NUM;
//   config.xclk_freq_hz = 10000000;
//   config.frame_size = FRAMESIZE_QVGA;
//   config.pixel_format = PIXFORMAT_JPEG; // for streaming
//   config.grab_mode = CAMERA_GRAB_WHEN_EMPTY;
//   config.fb_location = CAMERA_FB_IN_PSRAM;
//   config.jpeg_quality = 10;
//   config.fb_count = 1;
  
//   // camera init
//   esp_err_t err = esp_camera_init(&config);
//   if (err != ESP_OK) {
//     if(err==ESP_ERR_NOT_SUPPORTED){
//       config.pixel_format = PIXFORMAT_RGB565;
//       esp_err_t err = esp_camera_init(&config);
//       if (err != ESP_OK) {
//         Serial.printf("Camera init failed with error 0x%x", err);
//         return 0;
//       }
//     }
//   }

//   sensor_t * s = esp_camera_sensor_get();
//   // drop down frame size for higher initial frame rate
//   uint16_t pid = s->id.PID;
//   if(pid == OV2640_PID){
//     s->set_hmirror(s, 1);
//     s->set_vflip(s, 1);     
//   }
//   else if(pid == OV3660_PID){
//     s->set_hmirror(s, 1);
//     s->set_vflip(s, 0);     
//   }
//   else if(pid == GC2145_PID){
//     s->set_hmirror(s, 0);
//     delay(500);
//     s->set_vflip(s, 0);      
//   }
//   else if(pid == GC0308_PID){
//     s->set_hmirror(s, 0);
//     delay(500);
//     s->set_vflip(s, 0);     
//   }
//   else{
//     s->set_hmirror(s, 1);
//     s->set_vflip(s, 0);       
//   }
//   s->set_brightness(s, 1);  // Slightly increase brightness
//   s->set_saturation(s, 0);  // Reduce saturation
//   s->set_ae_level(s, -3);   // Set exposure compensation level
//   return 1;
// }

    Serial.println("camera 1");

    config.ledc_channel = LEDC_CHANNEL_0;

    config.fb_count = 1;

    Serial.println("camera 2");

    esp_err_t err = esp_camera_init(&config);

    Serial.println("camera 3");

    Serial.printf("err = 0x%x\n", err);
}

// void setup(){
//     Serial.begin(115200);
//     Serial.printf("start now ! here ! just here !\n");

//     esp_camera_init(&config);

//     SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
//     if (!SD_MMC.begin("/sdcard", true, true, SDMMC_FREQ_DEFAULT, 5)) {
//       Serial.println("Card Mount Failed");
//       return;
//     }
//     uint8_t cardType = SD_MMC.cardType();
//     if(cardType == CARD_NONE){
//         Serial.println("No SD_MMC card attached");
//         return;
//     }

//     Serial.print("SD_MMC Card Type: ");
//     if(cardType == CARD_MMC){
//         Serial.println("MMC");
//     } else if(cardType == CARD_SD){
//         Serial.println("SDSC");
//     } else if(cardType == CARD_SDHC){
//         Serial.println("SDHC");
//     } else {
//         Serial.println("UNKNOWN");
//     }

//     //SDカードの容量を表示
//     uint64_t cardSize = SD_MMC.cardSize() / (1024 * 1024);
//     Serial.printf("SD_MMC Card Size: %lluMB\n", cardSize);

//     listDir(SD_MMC, "/", 0);

//     createDir(SD_MMC, "/mydir");
//     listDir(SD_MMC, "/", 0);

//     removeDir(SD_MMC, "/mydir");
//     listDir(SD_MMC, "/", 2);

//     writeFile(SD_MMC, "/hello.txt", "Hello ");
//     appendFile(SD_MMC, "/hello.txt", "World!\n");
//     readFile(SD_MMC, "/hello.txt");

//     deleteFile(SD_MMC, "/foo.txt");
//     renameFile(SD_MMC, "/hello.txt", "/foo.txt");
//     readFile(SD_MMC, "/foo.txt");

//     testFileIO(SD_MMC, "/test.txt");
    
//     Serial.printf("Total space: %lluMB\r\n", SD_MMC.totalBytes() / (1024 * 1024));
//     Serial.printf("Used space: %lluMB\r\n", SD_MMC.usedBytes() / (1024 * 1024));

//     if (cameraSetup() == 1) {
//     Serial.println("Camera OK");
//     }
//     else {
//         Serial.println("Camera NG");
//         return;
//     }
// }

void setup() {
    Serial.begin(115200);
    Serial.println("1");

    SD_MMC.setPins(SD_MMC_CLK, SD_MMC_CMD, SD_MMC_D0);
    Serial.println("2");

    if (!SD_MMC.begin("/sdcard", true, true, SDMMC_FREQ_DEFAULT, 5)) {
        Serial.println("Card Mount Failed");
        return;
    }

    Serial.println("3");

    uint8_t cardType = SD_MMC.cardType();
    Serial.println("4");

    uint64_t cardSize = SD_MMC.cardSize();
    Serial.println("5");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()){

    serialinput = Serial.readStringUntil('\n');
    serialinput.trim();

    // if (serialinput.length() > 0) {
    //   stringComplete = true;
    // }

    if(serialinput == "shot"){
      shot = true;
    }
  }

  if(shot){
    camera_fb_t *fb = esp_camera_fb_get();

    if (fb == NULL) {
        Serial.println("Camera capture failed");
        return;
    }

    // String filename = "/image" + String(counter) + ".jpg";

    char filename[20];
    sprintf(filename, "/image%03d.jpg", counter);

    File file = SD_MMC.open(filename, FILE_WRITE);
    if (!file) {
    Serial.println("File open failed");
    esp_camera_fb_return(fb);
    shot = false;
    return;
    }
    
    file.write(fb->buf, fb->len);
    file.close();

    // if (SD_MMC.exists(filename.c_str())) {
    //   Serial.println("shot has done.");
    //   shot = false;
    //   counter = counter + 1;
    // }

    Serial.println("Saved.");
    counter++;
    shot = false;

    esp_camera_fb_return(fb);
  }
}
