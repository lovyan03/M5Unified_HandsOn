// #include <M5UnitRCA.h>      // UnitRCA を使う場合これを追加
// #include <M5UnitLCD.h>      // UnitLCD を使う場合これを追加
// #include <M5UnitOLED.h>     // UnitOLED を使う場合これを追加
// #include <M5UnitGLASS.h>    // UnitGLASS を使う場合これを追加
// #include <M5AtomDisplay.h>  // AtomDisplay を使う場合これを追加

#include <M5Unified.h>

void step();

void setup()
{
  auto cfg = M5.config(); // 設定用の構造体を取得。
//cfg.external_speaker.hat_spk = true;    // HAT SPK  を使う場合これを追加
//cfg.external_speaker.hat_spk2 = true;   // HAT SPK2 を使う場合これを追加
//cfg.external_speaker.atomic_spk = true; // ATOM SPK を使う場合これを追加

// 最初にbeginを実行します。M5Unifiedの準備に必ず必要です。
  M5.begin(cfg);

  // 実験用の関数を実行する。
  step();
}

void loop(void)
{
  delay(1);
  M5.update();

  // ボタンA,ボタンB,電源ボタン,画面のいずれかをクリックする度に実験用の関数を実行する。
  if (M5.BtnA.wasClicked() || M5.BtnB.wasClicked() || M5.BtnPWR.wasClicked() || M5.Touch.getDetail().wasClicked())
  {
    step();
  }
}

// 実験用の関数群
void step1();
void step2();
void step3();
void step4();
void step5();
void step6();

void step()
{
  static int step = 1;

  M5_LOGI("step:%d", step);
  // 操作音を鳴らす。
  M5.Speaker.tone(1000, 30);

  // 画面描画を高速化するおまじない
  M5.Display.startWrite();

  // 画面の表示をクリアする。(電子ペーパの場合は白、それ以外は黒)
  M5.Display.clear();

  // 実験用の関数を順番に試す。
  switch (step)
  {
  default: step = 1;
  case 1: step1(); break;
  case 2: step2(); break;
  case 3: step3(); break;
  case 4: step4(); break;
  case 5: step5(); break;
  case 6: step6(); break;
  }

  // startWriteと対でendWriteを使う。
  // UnitOLEDや電子ペーパーの場合はここで画面に反映される。
  M5.Display.endWrite();

  step += 1;
}

void step1()
{
  // 中心座標 20,60 の半径 30 の青い塗り潰しの円を描く。
  M5.Display.fillCircle(20, 60, 30, TFT_BLUE);
  // 青色の指定には TFT_BLUE という定数が使用できます。

  // 中心座標 60,20 の半径 30 の緑色の円を描く。
  M5.Display.drawCircle(60, 20, 30, TFT_GREEN);
}

void step2()
{
  // 左上座標 0, 20 から 幅 40, 高さ 60 の赤い塗り潰しの矩形のを描く。
  M5.Display.fillRect(0, 20, 40, 60, 0xFF0000u);
  // 0xFF0000u : この色の記述は 赤:255  緑:0  青:0 の意味になります。
  // 16進数の末尾に u を付けて unsigned型にすると、24bit の RGB888形式の色指定として扱われます。

  // 左上座標 10, 30 から 幅 50, 高さ 70 の緑色の矩形を描く。
  M5.Display.drawRect(10, 30, 50, 70, 0x07E0);
  // 0x07E0 : この色の記述は 赤:0  緑:255  青:0 の意味になります。
  // 16進数の末尾に u を付けない場合は 16bit の RGB565 形式の色指定として扱われます。
  // 16bit RGB565 指定では、赤5bit, 緑6bit, 青5bit の合計16bitになります。

  // 座標 10,30 から 座標 59,99 の位置まで青い直線を描く。
  M5.Display.drawLine(10, 30, 59, 99, (uint8_t)0x03);
  // (uint8_t)0x03 は 赤:0  緑:0  青:255 の意味になります。
  // uint8_t 型の色の指定は 8bit RGB332 形式の色指定として扱われます。
  // 8bit RGB332 形式では、赤3bit, 緑3bit, 青2bit の合計8bitになります。
}

void step3()
{
  // 三つの 座標 10,20   30,40   50,60 を頂点とする紫の塗り潰し三角を描く。
  M5.Display.fillTriangle(0, 20, 30, 80, 60, 20, TFT_PURPLE);

  // 三つの 座標 60,50   40,30   20,10 を頂点とするマゼンタの三角を描く。
  M5.Display.drawTriangle(0, 60, 30, 0, 60, 60, TFT_MAGENTA);
}

void step4()
{
  // 座標 40, 40 を中心に、半径と角度の異なる 3つの円弧を描く。
  M5.Display.drawArc(40, 40, 60, 50,   0, 120, TFT_RED);
  M5.Display.drawArc(40, 40, 50, 40, 120, 240, TFT_GREEN);
  M5.Display.fillArc(40, 40, 40, 30, 240, 360, TFT_BLUE);
}

void step5()
{
  // 画面端でのテキストの折り返しを無効にする。
  M5.Display.setTextWrap(false);

  // テキストのカーソル位置を左上に設定。
  M5.Display.setCursor(0, 0);

  for (int i = 1; i < 10; ++i)
  { // 文字の大きさを変更する。
    M5.Display.setTextSize(i);

    // printf関数でテキストを出力する。
    M5.Display.printf("size%d\n", i);
  }
}

void step6()
{
  int y = 5;
  for (int i = 1; i < 10; ++i)
  { // 文字の大きさを変更する。
    M5.Display.setTextSize(i);
    // drawString関数でテキストを出力する。
    M5.Display.drawString("text", 0, y);
    y += M5.Display.fontHeight();
  }
}
