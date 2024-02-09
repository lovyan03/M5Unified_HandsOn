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

  /// 電子ペーパーの場合は描画が速いモードに変更する。
  M5.Display.setEpdMode(m5gfx::epd_fast);

  /// ボタンの長押し判定の時間 (初期値500) を変更したい場合はこの関数で設定する。
// M5.BtnA.setHoldThresh(300); // 判定時間を300ミリ秒に変更。
// M5.BtnPWR.setHoldThresh(300); // 判定時間を300ミリ秒に変更。

  /// 実験用の関数を実行する。
  step();
}

void loop(void)
{
  M5.delay(10);

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
void step4();
void step5();
static constexpr int step_max = 5;

void step(int add)
{
  static int step = 0;

  /// 引数の指示に応じて実験関数の番号を変更する。
  step += add;
  if (step < 0) { step = step_max; }
  if (step > step_max) { step = 0; }

  /// 操作音を鳴らす。
  float Hz = 880 * powf(2.0, step / 12.0f);
  M5.Speaker.tone(Hz, 50);

  if (M5.Display.isEPD()) {
    M5.Display.fillScreen(TFT_BLACK);
    M5.Display.fillScreen(TFT_WHITE);
  }

  /// 画面描画を高速化するおまじない…。
  /// 描画を終える時に endWrite(); を呼出す。
  M5.Display.startWrite();

  /// 画面の表示をクリアする。(電子ペーパの場合は白、それ以外は黒)
  M5.Display.fillScreen(M5.Display.isEPD() ? TFT_WHITE : TFT_BLACK);

  /// 実験用の関数を順番に試す。
  switch (step)
  {
  default:
  case 0: step0(); break;
  case 1: step1(); break;
  case 2: step2(); break;
  case 3: step3(); break;
  case 4: step4(); break;
  case 5: step5(); break;
  }

  M5_LOGI("step:%d", step);

  /// startWriteと対でendWriteを使う。
  /// UnitOLEDや電子ペーパーの場合はここで画面に反映される。
  M5.Display.endWrite();
}

void step0()
{
  /// M5Stackの画面の中央に円を描いてみる。
  /// M5Stackの画面サイズは 320 x 240 なので、
  /// 中心座標 160,120 に半径120の円を描く。
  M5.Display.fillCircle(160, 120, 120, TFT_YELLOW);
}

void step1()
{
  /// step0の方法では、画面サイズが320x240の機種でしか中心に描画されない。
  /// step1では画面の大きさに基づいて中央に円を描いてみる。

  /// 画面幅 width の半分を x座標とする。
  int x = M5.Display.width() / 2;

  /// 画面高さ height の半分を y座標とする。
  int y = M5.Display.height() / 2;

  /// 円の半径は xとyの小さい方に合わせる。
  int r = x < y ? x : y;

  M5.Display.fillCircle(x, y, r, TFT_GREEN);
}

void step2()
{
  /// 画面を左右半分に分けて、左右に別の内容を描く。
  int wid = M5.Display.width() / 2;
  int hei = M5.Display.height();

  { /// 左半分の描画
    int x = wid / 2;
    int y = hei / 2;
    int r = x < y ? x : y;
    M5.Display.fillCircle(x, y, r, TFT_YELLOW);
  }

  { /// 右半分の描画
    int x = wid / 2;
    int y = hei / 2;
    int r = x < y ? x : y;
    M5.Display.fillCircle(x + wid, y, r, TFT_BLUE);
  }
}

void step3()
{
  /// step2の方法では、画面サイズが縦長の場合、縦に細長い領域に分割されてしまう。
  /// step3では画面の大きさに基づいて、なるべく正方形に近い領域に分割する。

  int wid = M5.Display.width();
  int hei = M5.Display.height();
  int area2_x = 0;
  int area2_y = 0;

  if (wid < hei)
  { /// 画面が縦に長い場合は上下に分割する。
    hei = hei / 2;
    area2_y = hei;
  }
  else
  { /// 正方形または横に長い場合は左右に分割する。
    wid = wid / 2;
    area2_x = wid;
  }

  { /// 左半分(または上半分)の描画
    int x = wid / 2;
    int y = hei / 2;
    int r = x < y ? x : y;
    M5.Display.fillCircle(x, y, r, TFT_RED);
  }

  { /// 右半分(または下半分)の描画
    int x = wid / 2;
    int y = hei / 2;
    int r = x < y ? x : y;
    M5.Display.fillCircle(x + area2_x, y + area2_y, r, TFT_CYAN);
  }
}

/// グラデーションで下の方の色が暗くなっていくバーを描く自作関数
void gradation_bar(int x, int y, int wid, int hei, uint32_t c)
{
  M5.Display.fillRect(x, y, wid, hei, TFT_BLACK);
  uint32_t r = (c>>16)&0xFF;
  uint32_t g = (c>> 8)&0xFF;
  uint32_t b = (c    )&0xFF;
  for (int i = 0; c && (i < hei); ++i)
  {
    M5.Display.fillRect(x, y + i, wid, 1, c);
    if (r) --r;
    if (g) --g;
    if (b) --b;
    c = (r << 16) | (g << 8) | b;
  }
}

/// ランダムなグラフっぽい絵を描く自作関数
void sample_graph(int x, int y, int wid, int hei, uint32_t c)
{
  M5.Display.drawRect(++x, ++y, --wid, --hei, TFT_BLACK);
  M5.Display.drawRect(x-1, y-1, wid, hei, TFT_WHITE);

  M5.Display.setClipRect(x, y, --wid, --hei);
  gradation_bar(x, y, wid, hei, c);

  int bar_w = (M5.Display.width() >> 5);
  if (bar_w <= 0) { bar_w = 1; }

  for (int i = 0; i < wid; i += bar_w)
  {
    int h = ((rand() & 255) * hei) >> 8;
    if (h)
    {
      gradation_bar(x + i, y + hei - h, bar_w, h, rand());
    }
  }
  M5.Display.clearClipRect();
}

void step4()
{
  /// 画面を分けて別々の描画をする例

  int wid = M5.Display.width();
  int hei = M5.Display.height();
  int area2_x = 0;
  int area2_y = 0;

  if (wid < hei)
  { /// 画面が縦に長い場合は上下に分割する。
    hei = hei / 2;
    area2_y = hei;
  }
  else
  { /// 正方形または横に長い場合は左右に分割する。
    wid = wid / 2;
    area2_x = wid;
  }

  /// 左半分(または上半分)の描画
  sample_graph(0, 0, wid, hei, 0x0055FFu);

  /// 右半分(または下半分)の描画
  sample_graph(area2_x, area2_y, wid, hei, 0xFF5500u);
}

void step5()
{
  /// step4 は 二等分にしていたのに対し、
  /// 1:2の比率で分けて別々の描画をする例

  int area1_w = M5.Display.width();
  int area1_h = M5.Display.height();
  int area2_x = 0;
  int area2_y = 0;
  int area2_w = area1_w;
  int area2_h = area1_h;

  if (area1_w < area1_h)
  { /// 画面が縦に長い場合は上下に分割する。
    area1_h /= 3;
    area2_h -= area1_h;
    area2_y = area1_h;
  }
  else
  { /// 正方形または横に長い場合は左右に分割する。
    area1_w /= 3;
    area2_w -= area1_w;
    area2_x = area1_w;
  }

  /// 左半分(または上半分)の描画
  sample_graph(0, 0, area1_w, area1_h, 0x00FF00u);

  /// 右半分(または下半分)の描画
  sample_graph(area2_x, area2_y, area2_w, area2_h, 0xFF00FFu);
}
