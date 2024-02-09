// #include <M5UnitRCA.h>      /// UnitRCA を使う場合、これを追加する。
// #include <M5UnitLCD.h>      /// UnitLCD を使う場合、これを追加する。
// #include <M5UnitOLED.h>     /// UnitOLED を使う場合、これを追加する。
// #include <M5UnitGLASS.h>    /// UnitGLASS を使う場合、これを追加する。
// #include <M5AtomDisplay.h>  /// AtomDisplay を使う場合、これを追加する。

#include <M5Unified.h>

void step(int add = 0);

void setup()
{
  /// 設定用の構造体を取得。
  auto cfg = M5.config();
//cfg.external_speaker.hat_spk = true;    /// HAT SPK  を使う場合、これを追加する。
//cfg.external_speaker.hat_spk2 = true;   /// HAT SPK2 を使う場合、これを追加する。
//cfg.external_speaker.atomic_spk = true; /// ATOM SPK を使う場合、これを追加する。

  /// M5Unifiedを使用する準備をする。
  M5.begin(cfg);

  /// 電子ペーパの場合は描画速度が最も速いモードに変更する。
  M5.Display.setEpdMode(m5gfx::epd_fastest);

  /// スピーカーから4000Hzの音を50ミリ秒間 出力する。
  M5.Speaker.tone(4000, 50);

  /// 画面にテキストを出力する。
  M5.Display.println("Hello M5Unified !");

  /// シリアルモニタにテキストを出力する。
  M5.Log.println("Hello M5Unified !");
}

void loop()
{
  /// 10ミリ秒待つ。
  M5.delay(10);

  /// ボタンやタッチ画面の操作状態を更新する。
  /// これを呼出さないと状態が変化しない。
  M5.update();

  /// タッチ画面の操作状態を取得する。
  auto touch = M5.Touch.getDetail();

  /// ボタンA、電源ボタン、タッチ画面のいずれかを短く押したか判定。
  if (M5.BtnA.wasClicked() || M5.BtnPWR.wasClicked() || (touch.wasClicked() && touch.base_y < M5.Display.height()))
  {
    /// スピーカーに1600Hzの音を500ミリ秒出力する。
    M5.Speaker.tone(1600, 500);

    /// 画面にテキストを出力する。
    M5.Display.println("Clicked !");

    /// シリアルモニタにテキストを出力する。
    M5.Log.println("Clicked !");
  }
}
