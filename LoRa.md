this md is memo about LoRa in plnq/spcb.

* esp32 dev module (秋月電子購入)でLoRaができなかった理由がわからない

サンプルコードを実行すると
```
#response
0xff 0xff 0xff
```
が帰ってきた．

- 別にノイズって感じでもない．: 毎回これが帰ってくるし，きっちり3バイトで5バイトとかになることがなかったから明らかに意図的．
- なんかconfigが違うのかな．．．

↓このサイトではできたらしい
https://qiita.com/Ken_____/items/04d525815ebd73987e5c