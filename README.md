# image_and_LoRa
うんち

- test1,test2は各スケッチ例から機能別に試験をしています．
  - カメラの画像首都
  - Loraの送信(送信のみ)
  - Loraの送受信
  - esp32同士でのuart通信
- test3からは上記のプログラムのパーツをまとめて本コードを作っていくつもりです．(今のところ)
- `esp32_22sr2_22~` のファイルはカメラ付きesp32のサンプルコード(スケッチ例)です．

---

-test1

- espuart : esp32同士でのテキストのuart通信

- lora_test(successed) : loraの送受信テスト\\

  -gro_2 : カメラ付きesp23が地上局としたときのコード(payloadと変わらない．たぶん)\\

  -ground : 私の私物のesp32 dev moduleを地上局にした時のコード(なぜか受信できず失敗)\\

   -payload : カメラ付きesp32のコード\\

- test1 : カメラ動かすコード

---

-test2

- uart_bin : バイナリデータをuart通信するよっていうコードだと思う．
`sky=payload`
