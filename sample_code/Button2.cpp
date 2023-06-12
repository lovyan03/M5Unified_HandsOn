#include <M5UnitLCD.h>
#include <M5UnitOLED.h>
#include <M5UnitGLASS.h>

#include <M5Unified.h>

// この記述で "M5.Display" の代わりに "dsp" と書くことができるようになる。
auto &dsp = M5.Display;

// この記述で "M5.BtnA" の代わりに "btn" と書くことができるようになる。
// 操作対象のボタンを変更したい場合はここの記述を変更する。
auto &btn = M5.BtnA;

// この記述で "M5.Speaker" の代わりに "spk" と書くことができるようになる。
auto &spk = M5.Speaker;

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
  static int Hz = 1000;
  delay(1);

  M5.update();

  // 1回クリック時
  if (btn.wasSingleClicked())
  {
    spk.tone(500, 500, 0);
    dsp.fillScreen(TFT_YELLOW);
  }

  // 2回クリック時
  if (btn.wasDoubleClicked())
  {
    spk.tone(1500, 500, 0);
    dsp.fillScreen(TFT_BLUE);
  }

  // 長押し状態になった時
  if (btn.wasHold())
  {
    dsp.fillScreen(TFT_BLACK);
  }

  // 長押し中
  if (btn.isHolding())
  {
    // 長押し前に単押しを何回していたか分岐
    switch (btn.getClickCount())
    {
    case 0: // 普通に長押しを始めた場合
      if (Hz < 10000) { Hz += 1;} // 音程を上げる
      break;

    default: // 1回以上 短押し後に長押しを始めた場合
      if (Hz > 10) { Hz -= 1; }  // 音程を下げる
      break;
    }
    spk.tone(Hz, 500, 0);
  }

  // ディスプレイが描画待ちになっているか判定する
  // (電子ペーパの場合ここで描画をスキップすることがある)
  if (!dsp.displayBusy())
  { // 電子ペーパやUnitOLEDの場合はこのタイミングで画面に反映される。
    dsp.display();
  }
}
