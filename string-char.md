## `String`と`const char *`は別物

例えば
```cpp
String s = "Hello";
```
この`String`はArduinoが用意している**文字列クラス**です。

一方、昔ながらのC言語では文字列は
```cpp
char str[] = "Hello";
const char *str = "Hello";
```

のように扱います。

つまり

* `String` … Arduinoの便利な文字列クラス
* `const char *` … C言語形式の文字列

は別の型です。

---

## `writeFile()`が欲しいのは`const char *`

ヘッダを見ると

```cpp
void writeFile(fs::FS &fs, const char * path, const char * message);
```

となっています。

```cpp
String inputString = "abc";
```

があっても

```cpp
writeFile(SD_MMC, "/a.txt", inputString);
```

とは書けません。


```cpp
writeFile(SD_MMC, "/a.txt", inputString.c_str());
```

とします。

---

## `c_str()`とは

`c_str()`は

> **Stringの中身をC言語の文字列（const char*）として返す関数**

例えば

```cpp
String s = "Hello";
Serial.println(s);
```

は`String`を表示しています。

一方

```cpp
Serial.println(s.c_str());
```

は`const char *`を表示しています。

どちらも

```
Hello
```

と表示されます。

---

## イメージ

`String`の中では

```
String
 ┌──────────┐
 │ Hello    │
 └──────────┘
```

のようにデータを管理しています。

`c_str()`を呼ぶと

```
"Hello"
 ^
 |
const char *
```

という**中身へのポインタ**を返します。データをコピーしているわけではありません。

---

## `inputString + "\n"`は？

例えば

```cpp
inputString = "abc";
inputString + "\n"
```

なら
```
abc\n
```

という**新しいString**ができます。

さらに

```cpp
(inputString + "\n").c_str()
```

で

```
"abc\n"
```

というC文字列として`appendFile()`へ渡しています。

---

>## 覚えておけば十分なこと

Arduinoでは

```cpp
String
```

で文字列を作って、

C言語形式の文字列を要求される関数に渡すときだけ

```cpp
.c_str()
```

を付ける、と覚えておけばまず困りません。

---

### 逆に知っておくと便利なこと

実は、ESP32のArduino環境では

```cpp
File file = SD_MMC.open("/log.txt", FILE_APPEND);
file.println(inputString);
```

のように`File`クラスの`print()`や`println()`は`String`をそのまま受け取れるので、**`c_str()`を書く必要はありません**。

そのため、ESP32でSDカードを扱う場合は、`appendFile()`のラッパー関数よりも`File`クラスを直接使う書き方のほうが自然で柔軟なことが多いです。
