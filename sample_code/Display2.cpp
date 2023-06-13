// #include <M5UnitRCA.h>      /// UnitRCA を使う場合これを追加
// #include <M5UnitLCD.h>      /// UnitLCD を使う場合これを追加
// #include <M5UnitOLED.h>     /// UnitOLED を使う場合これを追加
// #include <M5UnitGLASS.h>    /// UnitGLASS を使う場合これを追加
// #include <M5AtomDisplay.h>  /// AtomDisplay を使う場合これを追加

#include <M5Unified.h>

void step();

void setup()
{
  auto cfg = M5.config(); /// 設定用の構造体を取得。
//cfg.external_speaker.hat_spk = true;    /// HAT SPK  を使う場合これを追加
//cfg.external_speaker.hat_spk2 = true;   /// HAT SPK2 を使う場合これを追加
//cfg.external_speaker.atomic_spk = true; /// ATOM SPK を使う場合これを追加

  /// 最初にbeginを実行します。M5Unifiedの準備に必ず必要です。
  M5.begin(cfg);

  /// 実験用の関数を実行する。
  step();
}

void loop(void)
{
  delay(1);
  M5.update();

  /// ボタンA,ボタンB,電源ボタン,画面のいずれかをクリックする度に実験用の関数を実行する。
  if (M5.BtnA.wasClicked() || M5.BtnB.wasClicked() || M5.BtnPWR.wasClicked() || M5.Touch.getDetail().wasClicked())
  {
    step();
  }
}

/// 実験用の関数群
void step1();
void step2();
void step3();
void step4();
void step5();

void step()
{
  static int step = 1;

  M5_LOGI("step:%d", step);
  // 操作音を鳴らし、画面全体を黒で塗りつぶす
  M5.Speaker.tone(1000, 50);

  M5.Display.startWrite();
  M5.Display.fillScreen(TFT_BLACK);

  // 実験用の関数を順番に試す。
  switch (step)
  {
  default: step = 1;
  case 1: step1(); break;
  case 2: step2(); break;
  case 3: step3(); break;
  case 4: step4(); break;
  }

  M5.Display.endWrite();

  step += 1;
}

void step1()
{
  // M5Stackの画面の中央に円を描いてみる。
  // M5Stackの画面サイズは 320 x 240 なので、
  // 中心座標 160,120 に半径120の円を描く。
  M5.Display.fillCircle(160, 120, 120, TFT_YELLOW);
}

void step2()
{
  // step1の方法では、画面サイズが320x240の機種でしか中心に描画されない。
  // step2では画面の大きさに基づいて中央に円を描いてみる。

  // 画面幅 width の半分を x座標とする。
  int x = M5.Display.width() / 2;

  // 画面高さ height の半分を y座標とする。
  int y = M5.Display.height() / 2;

  // 円の半径は xとyの小さい方に合わせる。
  int r = x < y ? x : y;

  M5.Display.fillCircle(x, y, r, TFT_WHITE);
}

void step3()
{
  // 画面を左右半分に分けて、左右に別の内容を描く。
  int wid = M5.Display.width() / 2;
  int hei = M5.Display.height();

  { // 左半分の描画
    int x = wid / 2;
    int y = hei / 2;
    int r = x < y ? x : y;
    M5.Display.fillCircle(x, y, r, TFT_RED);
  }

  { // 右半分の描画
    int x = wid / 2;
    int y = hei / 2;
    int r = x < y ? x : y;
    M5.Display.fillCircle(x + wid, y, r, TFT_BLUE);
  }
}

void step4()
{
  // 画面を左右半分に分けて、左右に別の内容を描く。
  int wid = M5.Display.width() / 2;
  int hei = M5.Display.height();

  { // 左半分の描画
    int x = wid / 2;
    int y = hei / 2;
    int r = x < y ? x : y;
    M5.Display.fillCircle(x, y, r, TFT_RED);
  }

  { // 右半分の描画
    int x = wid / 2;
    int y = hei / 2;
    int r = x < y ? x : y;
    M5.Display.fillCircle(x + wid, y, r, TFT_BLUE);
  }
}
