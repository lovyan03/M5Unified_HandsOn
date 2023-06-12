// #include <M5UnitRCA.h>      // UnitRCA を使う場合これを追加
// #include <M5UnitLCD.h>      // UnitLCD を使う場合これを追加
// #include <M5UnitOLED.h>     // UnitOLED を使う場合これを追加
// #include <M5UnitGLASS.h>    // UnitGLASS を使う場合これを追加
// #include <M5AtomDisplay.h>  // AtomDisplay を使う場合これを追加

#include <M5Unified.h>
void setup()
{
  auto cfg = M5.config(); // 設定用の構造体を取得。
//cfg.external_speaker.hat_spk = true;    // HAT SPK  を使う場合これを追加
//cfg.external_speaker.hat_spk2 = true;   // HAT SPK2 を使う場合これを追加
//cfg.external_speaker.atomic_spk = true; // ATOM SPK を使う場合これを追加

// 最初にbeginを実行します。M5Unifiedの準備に必ず必要です。
  M5.begin(cfg);

// スピーカーに音を出力します。
  M5.Speaker.tone(4000, 50);

// 画面にテキストを出力します。
  M5.Display.println("Hello M5Unified !");

// シリアルモニタにテキストを出力します。
  M5.Log.println("Hello M5Unified !");
}

void loop()
{
  delay(1);

// ボタンの操作状態を更新します。これを呼出すまでボタンの状態は変化しません。
  M5.update();

// ボタンA、電源ボタン、タッチパネルのいずれかを短く押したか判定。
  if (M5.BtnA.wasClicked() || M5.BtnPWR.wasClicked() || M5.Touch.getDetail().wasClicked())
  {
// スピーカーに音を出力します。
    M5.Speaker.tone(1600, 500);

// 画面にテキストを出力します。
    M5.Log.println("Clicked !");

// シリアルモニタにテキストを出力します。
    M5.Display.println("Clicked !");
  }
}
