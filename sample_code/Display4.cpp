// #include <M5UnitRCA.h>      /// UnitRCA を使う場合、これを追加する。
// #include <M5UnitLCD.h>      /// UnitLCD を使う場合、これを追加する。
// #include <M5UnitOLED.h>     /// UnitOLED を使う場合、これを追加する。
// #include <M5UnitGLASS.h>    /// UnitGLASS を使う場合、これを追加する。
// #include <M5AtomDisplay.h>  /// AtomDisplay を使う場合、これを追加する。

#include <M5Unified.h>

void step(int add = 0);

M5Canvas canvas[2];
int canvas_index = 0;

void setup()
{
  /// 設定用の構造体を取得。
  auto cfg = M5.config();
//cfg.external_speaker.hat_spk = true;    /// HAT SPK  を使う場合、これを追加する。
//cfg.external_speaker.hat_spk2 = true;   /// HAT SPK2 を使う場合、これを追加する。
//cfg.external_speaker.atomic_spk = true; /// ATOM SPK を使う場合、これを追加する。

  /// M5Unifiedを使用する準備をする。
  M5.begin(cfg);

  /// 電子ペーパの場合は描画がはやいモードに変更する。
  M5.Display.setEpdMode(m5gfx::epd_fast);

  /// ボタンの長押し判定の時間 (初期値500) を変更したい場合はこの関数で設定する。
// M5.BtnA.setHoldThresh(300); // 判定時間を300ミリ秒に変更。
// M5.BtnPWR.setHoldThresh(300); // 判定時間を300ミリ秒に変更。

  for (int i = 0; i < 2; ++i)
  {
    canvas[i].setPsram(false);
    canvas[i].createSprite(M5.Display.width(), 40);
  }

  /// 実験用の関数を実行する。
  step();
}

void loop(void)
{
  /// 操作内容に応じて変更する変数。
  /// 実験用の関数の実行順序に影響する。 1=次に進む / -1=前に戻る / 0=もう一度。
  int add = 0;

  /// ボタン操作・タッチ操作の状態を更新する。
  M5.update();

  /// タッチ操作の情報を取得する。
  auto touch_detail = M5.Touch.getDetail();

  if (touch_detail.wasClicked() && touch_detail.base_y < M5.Display.height())
  { /// タッチ画面がクリックされた？
    /// 画面の左・中央・右のどのエリアがクリックされたかに応じて変更する。
    add = 1 - (M5.Touch.getDetail().x * 3 / M5.Display.width());
    /// 操作された場所が…
    ///  右側 = -1 = 前に戻る。
    ///  中央 =  0 = もう一度。
    ///  左側 =  1 = 次に進む。
  }
  else if (M5.BtnA.wasSingleClicked() || M5.BtnPWR.wasSingleClicked())
  { /// ボタンAか電源ボタンが1度クリックされた？
    /// 次の実験用の関数に進む。
    add = 1;
  }
  else if (M5.BtnA.wasDoubleClicked() || M5.BtnPWR.wasDoubleClicked() || M5.BtnC.wasClicked())
  { /// ボタンAか電源ボタンが2度クリックされた または ボタンCがクリックされた？
    /// 前の実験用の関数に戻る。
    add = -1;
  }
  else if (M5.BtnA.wasHold() || M5.BtnPWR.wasHold() || M5.BtnB.wasClicked())
  { /// ボタンAか電源ボタンが長押しされた または ボタンBがクリックされた？
    /// 同じ実験用の関数をもう一度。
    add = 0;
  }
  else
  { /// どの操作もされていなければ終了。
    M5.delay(1);
    return;
  }

  /// 操作に応じて実験用の関数を試す。
  step(add);
}

/// 実験用の関数群
void step0();
void step1();
void step2();
void step3();

void step(int add)
{
  static int step = 0;

  /// 引数の指示に応じて実験関数の番号を変更する。
  step += add;
  if (step < 0) { step = 3; }
  if (step > 3) { step = 0; }

  /// 操作音を鳴らす。
  float Hz = 880 * powf(2.0, step / 12.0f);
  M5.Speaker.tone(Hz, 50);

  /// 画面の表示をクリアする。(電子ペーパの場合は白、それ以外は黒)
  M5.Display.fillScreen(M5.Display.isEPD() ? TFT_WHITE : TFT_BLACK);

  uint32_t msec = millis();
  /// 実験用の関数を順番に試す。
  switch (step)
  {
  default:
  case 0: step0(); break;
  case 1: step1(); break;
  case 2: step2(); break;
  case 3: step3(); break;
  }
  msec = millis() - msec;

  M5_LOGI("step:%d   %d msec", step, msec);
}

void step0()
{
  int x_count = M5.Display.width() / (M5.Display.isEPD() ? 66 : 2);
  int y_count = M5.Display.height() / (M5.Display.isEPD() ? 66 : 2);

  int base_color = rand();

  for (int i = 0; i < y_count; ++i)
  {
    int y = i * M5.Display.height() / y_count;
    int h = (i+1) * M5.Display.height() / y_count - y;
    for (int j = 0; j < x_count; ++j)
    {
      int x = j * M5.Display.width() / x_count;
      int w = (j+1) * M5.Display.width() / x_count - x;
      uint32_t color = ((base_color + (i << 3)) & 0x0000FF)
                     +  (base_color +((i+j)<<10)& 0x00FF00)
                     +  (base_color + (j << 18) & 0xFF0000);
      M5.Display.fillRect(x, y, w, h, color);
    }
  }
}

void step1()
{
  M5.Display.startWrite();
  step0();
  M5.Display.endWrite();
}

void step2()
{
  int x_count = M5.Display.width() / (M5.Display.isEPD() ? 66 : 2);
  int y_count = M5.Display.height() / (M5.Display.isEPD() ? 66 : 2);

  int base_color = rand();

  int y_pos = 0;
  do
  {
    int i_start = y_pos * y_count / M5.Display.height();
    int i_end   = (y_pos+canvas[canvas_index].height()) * y_count / M5.Display.height();

    for (int i = i_start; i <= i_end; ++i)
    {
      int y = i * M5.Display.height() / y_count;
      int h = (i+1) * M5.Display.height() / y_count - y;
      for (int j = 0; j < x_count; ++j)
      {
        int x = j * M5.Display.width() / x_count;
        int w = (j+1) * M5.Display.width() / x_count - x;

        uint32_t color = ((base_color + (i << 3)) & 0x0000FF)
                       +  (base_color +((i+j)<<10)& 0x00FF00)
                       +  (base_color + (j << 18) & 0xFF0000);
        canvas[canvas_index].fillRect(x, y - y_pos, w, h, color);
      }
    }
    canvas[canvas_index].pushSprite(&M5.Display, 0, y_pos);
    y_pos += canvas[canvas_index].height();
    canvas_index = 1 - canvas_index;
  } while (y_pos < M5.Display.height());
}

void step3()
{
  M5.Display.startWrite();
  step2();
  M5.Display.endWrite();
}
