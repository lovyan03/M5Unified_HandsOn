// #include <M5UnitRCA.h>      /// UnitRCA を使う場合これを追加
// #include <M5UnitLCD.h>      /// UnitLCD を使う場合これを追加
// #include <M5UnitOLED.h>     /// UnitOLED を使う場合これを追加
// #include <M5UnitGLASS.h>    /// UnitGLASS を使う場合これを追加
// #include <M5AtomDisplay.h>  /// AtomDisplay を使う場合これを追加

#include <M5Unified.h>

void step(int add = 0);

void setup()
{
  auto cfg = M5.config(); /// 設定用の構造体を取得。
//cfg.external_speaker.hat_spk = true;    /// HAT SPK  を使う場合これを追加
//cfg.external_speaker.hat_spk2 = true;   /// HAT SPK2 を使う場合これを追加
//cfg.external_speaker.atomic_spk = true; /// ATOM SPK を使う場合これを追加

  /// 最初にbeginを実行する。M5Unifiedの準備に必ず必要。
  M5.begin(cfg);

  /// ボタンの長押し判定の時間 (初期値500) を変更したい場合はこの関数で設定する。
  M5.BtnA.setHoldThresh(300); // 判定時間を300ミリ秒に変更。
  M5.BtnPWR.setHoldThresh(300); // 判定時間を300ミリ秒に変更。

  /// 電子ペーパの場合は描画速度が最も速いモードに変更する。
  M5.Display.setEpdMode(m5gfx::epd_fastest);

  /// 実験用の関数を実行する。
  step();
}

void loop(void)
{
  delay(10);

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
void step6();

void step(int add)
{
  static int step = 0;

  /// 引数の指示に応じて実験関数の番号を変更する。
  step += add;

  /// 画面描画を高速化するおまじない…。
  /// 描画を終える時に endWrite(); を呼出す。
  M5.Display.startWrite();

  /// 画面の表示をクリアする。(電子ペーパの場合は白、それ以外は黒)
  M5.Display.fillScreen(M5.Display.isEPD() ? TFT_WHITE : TFT_BLACK);

  /// 実験用の関数を順番に試す。
  switch (step)
  {
  default: step = 0;
  case 0: step0(); break;
  case 1: step1(); break;
  case 2: step2(); break;
  case 3: step3(); break;
  case 4: step4(); break;
  case -1: step = 5;
  case 5: step5(); break;
  }

  M5_LOGI("step:%d", step);

  /// 操作音を鳴らす。
  float Hz = 880 * powf(2.0, step / 12.0f);
  M5.Speaker.tone(Hz, 50);

  /// startWriteと対でendWriteを使う。
  /// UnitOLEDや電子ペーパーの場合はここで画面に反映される。
  M5.Display.endWrite();
}

void step0()
{
  /// 中心座標 20,60 の半径 30 の青い塗り潰しの円を描く。
  M5.Display.fillCircle(20, 60, 30, TFT_BLUE);
  /// 青色の指定には TFT_BLUE という定数が使用できます。

  /// 中心座標 60,20 の半径 30 の緑色の円を描く。
  M5.Display.drawCircle(60, 20, 30, TFT_GREEN);
}

void step1()
{
  /// 左上座標 0, 20 から 幅 40, 高さ 60 の赤い塗り潰しの矩形のを描く。
  M5.Display.fillRect(0, 20, 40, 60, 0xFF0000u);
  /// 0xFF0000u : この色の記述は 赤:255  緑:0  青:0 の意味になります。
  /// 16進数の末尾に u を付けて unsigned型にすると、24bit の RGB888形式の色指定として扱われます。

  /// 左上座標 10, 30 から 幅 50, 高さ 70 の緑色の矩形を描く。
  M5.Display.drawRect(10, 30, 50, 70, 0x07E0);
  /// 0x07E0 : この色の記述は 赤:0  緑:255  青:0 の意味になります。
  /// 16進数の末尾に u を付けない場合は 16bit の RGB565 形式の色指定として扱われます。
  /// 16bit RGB565 指定では、赤5bit, 緑6bit, 青5bit の合計16bitになります。

  /// 座標 10,30 から 座標 59,99 の位置まで青い直線を描く。
  M5.Display.drawLine(10, 30, 59, 99, (uint8_t)0x03);
  /// (uint8_t)0x03 は 赤:0  緑:0  青:255 の意味になります。
  /// uint8_t 型の色の指定は 8bit RGB332 形式の色指定として扱われます。
  /// 8bit RGB332 形式では、赤3bit, 緑3bit, 青2bit の合計8bitになります。
}

void step2()
{
  /// 三つの 座標 10,20   30,40   50,60 を頂点とする紫の塗り潰し三角を描く。
  M5.Display.fillTriangle(0, 20, 30, 80, 60, 20, TFT_PURPLE);

  /// 三つの 座標 60,50   40,30   20,10 を頂点とするマゼンタの三角を描く。
  M5.Display.drawTriangle(0, 60, 30, 0, 60, 60, TFT_MAGENTA);
}

void step3()
{
  /// 座標 40, 40 を中心に、半径と角度の異なる 3つの円弧を描く。
  M5.Display.drawArc(40, 40, 60, 50,   0, 120, TFT_RED);
  M5.Display.drawArc(40, 40, 50, 40, 120, 240, TFT_GREEN);
  M5.Display.fillArc(40, 40, 40, 30, 240, 360, TFT_BLUE);
}

void step4()
{
  int y = 5;
  for (int i = 1; i < 10; ++i)
  { /// 文字の大きさを変更する。
    M5.Display.setTextSize(i);
    /// drawString関数でテキストを出力する。
    /// drawStringはカーソルの影響を受けず、指定した座標に表示される。
    M5.Display.drawString("hello", 0, y);
    y += M5.Display.fontHeight();
  }
}

void step5()
{
  /// 画面端でのテキストの折り返しを無効にする。
  M5.Display.setTextWrap(false);

  /// テキストのカーソル位置を左上に設定。
  M5.Display.setCursor(0, 0);

  for (int i = 1; i < 10; ++i)
  { /// 文字の大きさを変更する。
    M5.Display.setTextSize(i);

    /// printf関数でテキストを出力する。
    /// 最後に \n 改行があるので、カーソル位置が次の行の左端に移動する。
    M5.Display.printf("size%d\n", i);
  }
}
