#include <M5UnitLCD.h>
#include <M5UnitOLED.h>
#include <M5UnitGLASS.h>

#include <M5Unified.h>

// この記述で "M5.Display" の代わりに "dsp" と書くことができるようになる。
auto &dsp = M5.Display;

// この記述で "M5.BtnA" の代わりに "btn" と書くことができるようになる。
// 操作対象のボタンを変更したい場合はここの記述を変更する。
auto &btn = M5.BtnA;

// CoreInk,Core2,CoreS3,Tough,StickC/CPlus の場合は M5.BtnPWR で電源ボタンも使用できる。
// auto &btn = M5.BtnPWR;
// ※ 但しCoreInk以外の機種の BtnPWR については、電源管理ICがボタンを管理しており、
//    今ボタンが押された状態かどうかは判定ができない。
//    使える機能は wasClick / wasHold および連続クリック回数に関する機能のみに限定される。

void setup(void)
{
  M5.begin();

// 電子ペーパの場合は描画速度が最も速いモードに変更する。
  dsp.setEpdMode(m5gfx::epd_fastest);

// 画面が縦長の場合は横長配置に変更する。
  if (dsp.width() < dsp.height())
  {
    dsp.setRotation(dsp.getRotation() ^ 1);
  }

// ボタンの長押し判定の時間 (初期値500) を変更したい場合はこの関数で設定する。
//  btn.setHoldThresh(300); // 判定時間を300ミリ秒に変更

  // 描画処理の前後を startWrite / endWrite で囲うことで、
  // 描画時間を短縮する効果がある。
  dsp.startWrite();
}

void loop(void)
{
  delay(50);

  M5.update();

  // ボタンの状態取得関数の結果をdata配列にまとめる。
  uint8_t data[] = {
    btn.isReleased(),
    btn.isPressed(),
    btn.wasReleased(),
    btn.wasPressed(),
    btn.wasClicked(),
    btn.wasHold(),
    btn.wasReleasedAfterHold(),
    btn.isHolding(),
    btn.wasSingleClicked(),
    btn.wasDoubleClicked(),
    btn.wasDecideClickCount(),
  };

  // 反応があった時・無かった時の表示色を設定。
  // 電子ペーパの時とそれ以外で色を分ける。
  int colors[] = {
    dsp.isEPD() ? TFT_WHITE : TFT_BLACK,
    dsp.isEPD() ? TFT_BLACK : TFT_WHITE,
  };

  static int x;
  int y = 0;
  int h = dsp.height() / sizeof(data);
  int w = (dsp.width() > 320) ? 3 : 2;

  for (auto d: data)
  {
    // 画面に出力
    dsp.fillRect(x, y, w, h - 1, colors[d]);
    y += h;
    // ログ(シリアルモニタ)にも出力
    M5.Log.printf("%d ", d);
  }
  // 描画位置を右に移動、画面端に達したら左端に戻る。
  if ((x += w) >= dsp.width()) { x = 0; }

  // ディスプレイが描画待ちになっているか判定する
  // (電子ペーパの場合ここで描画をスキップすることがある)
  if (!dsp.displayBusy())
  {
    // 新しい位置に縦線を描画する。
    dsp.fillRect(x, 0, w, h * sizeof(data), TFT_YELLOW);

    // 電子ペーパやUnitOLEDの場合はこのタイミングで画面に反映される。
    dsp.display();
  }

  // ログ(シリアルモニタ)にクリック回数と改行を出力
  M5.Log.printf(" / %d \n", btn.getClickCount());
}
