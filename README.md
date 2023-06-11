# M5Unifiedハンズオン

このリポジトリは`M5Unified`を実際に使って試す、ハンズオン イベント用の資料、サンプルコードの公開用です。

## まず環境構築

[こちらの手順](https://docs.m5stack.com/ja/quick_start/m5unified/intro_vscode)に従い、`Visual Studio Code` + `PlatformIO` を使用できる状態にします。<br>

https://docs.m5stack.com/ja/quick_start/m5unified/intro_vscode


## M5Unifiedハンズオンの準備

進行用のスライドは[こちら](https://docs.google.com/presentation/d/1a_PSTQC-5HFejekAp_ExxMcwR8apyax_pxPR6dsnbnY/) にあります。<br>

https://docs.google.com/presentation/d/1a_PSTQC-5HFejekAp_ExxMcwR8apyax_pxPR6dsnbnY/


---

 - [最初に試してみるサンプルコード](Start/Start.cpp)

## 最初に試してみるサンプルコード

```cpp
//#include <M5UnitLCD.h>    // UnitLCD   を使う場合これを追加
//#include <M5UnitOLED.h>   // UnitOLED  を使う場合これを追加
//#include <M5UnitGLASS.h>  // UnitGLASS を使う場合これを追加

#include <M5Unified.h>

void setup() {
  auto cfg = M5.config();
//cfg.external_speaker.hat_spk = true;    // HAT SPK  を使う場合これを追加
//cfg.external_speaker.hat_spk2 = true;   // HAT SPK2 を使う場合これを追加
//cfg.external_speaker.atomic_spk = true; // ATOM SPK を使う場合これを追加

  M5.begin(cfg);
  M5.Display.println("Hello M5Unified !");
}

void loop() {
  delay(1);
  M5.update();
  if (M5.BtnA.wasClicked() || M5.BtnPWR.wasClicked() || M5.Touch.getDetail().wasClicked()) {
    M5.Speaker.tone(1600, 500);
    M5.Log.println("Click !");
    M5.Display.println("Click !");
  }
}
```

 - 画面の左上に `Hello M5Unified !` が表示されましたか？
 - Aボタン または タッチパネルを **短押し** した時に音が鳴りますか？


---

 - [ボタンのサンプルコード1](Button1/Button1.cpp)
 - [ボタンのサンプルコード2](Button2/Button2.cpp)

