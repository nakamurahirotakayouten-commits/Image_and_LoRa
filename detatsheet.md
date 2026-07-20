「**Notes for GPIO**」には、このESP32-S3 WROOMボードでGPIOを使う際の注意点がまとめられています。要約すると次の内容です。([docs.freenove.com][1])

### 1. Strapping Pin（起動設定ピン）

ESP32-S3には以下の4本のストラッピングピンがあります。

* GPIO0
* GPIO3
* GPIO45
* GPIO46

これらは**リセットや電源投入時にブート設定を決めるピン**です。

* 電源投入時にHigh/Lowが読み取られ、起動モードなどが決まる
* 内部プルアップ／プルダウンがある
* 起動後は通常のGPIOとして使用可能

つまり、**起動中だけ特別な役割を持つGPIO**なので、外部回路を接続するときは注意が必要です。([docs.freenove.com][1])

---

### 2. PSRAM Pin

このボードは8MB PSRAMを搭載しています。

* **GPIO35～GPIO37** はOPI PSRAMで使用される
* PSRAM使用時は他用途には使えない
* PSRAMを使わない場合のみ通常GPIOとして利用可能

([docs.freenove.com][1])

---

### 3. SD Card Pin

基板裏面のSDカードスロットは

* **GPIO38**
* **GPIO39**
* **GPIO40**

を使用しています。

またSDカードはArduinoの **SD_MMC.h** ライブラリを使った**1-bit SDMMCモード**で動作します。([docs.freenove.com][1])

---

### 4. USB Pin

**MicroPythonでは**

* GPIO19
* GPIO20

がUSB専用として使われるため、**他の用途には使用できません。**([docs.freenove.com][1])

---

### 5. Camera Pin

カメラは以下のGPIOを占有しています。

| カメラ信号 | GPIO   |
| ----- | ------ |
| SIOD  | GPIO4  |
| SIOC  | GPIO5  |
| VSYNC | GPIO6  |
| HREF  | GPIO7  |
| Y4    | GPIO8  |
| Y3    | GPIO9  |
| Y5    | GPIO10 |
| Y2    | GPIO11 |
| Y6    | GPIO12 |
| PCLK  | GPIO13 |
| XCLK  | GPIO15 |
| Y9    | GPIO16 |
| Y8    | GPIO17 |
| Y7    | GPIO18 |

これらは**カメラ使用中は他の用途に使わないようにしてください**。([docs.freenove.com][1])

---

### 実際に自由に使いやすいGPIO

もし**カメラ・SDカード・USB・PSRAMを使う予定がある**なら、比較的使いやすいのは例えば

* GPIO1
* GPIO2
* GPIO14
* GPIO21
* GPIO47
* GPIO48

などです（GPIO0/3/45/46はストラッピングピンなので起動時は注意）。([docs.freenove.com][1])

もし「**このボードでLEDやスイッチをつなぐなら、どのGPIOを選ぶのが安全か**」という観点で知りたい場合は、その用途に合わせておすすめのGPIOもお伝えできます。

[1]: https://docs.freenove.com/projects/fnk0085/en/latest/fnk0085/codes/C/Preface.html?utm_source=chatgpt.com "Preface — fnk0085-docs v1.0.0 documentation"
