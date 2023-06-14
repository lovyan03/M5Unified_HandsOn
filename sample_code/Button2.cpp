// #include <M5UnitRCA.h>      /// UnitRCA を使う場合、これを追加する。
// #include <M5UnitLCD.h>      /// UnitLCD を使う場合、これを追加する。
// #include <M5UnitOLED.h>     /// UnitOLED を使う場合、これを追加する。
// #include <M5UnitGLASS.h>    /// UnitGLASS を使う場合、これを追加する。
// #include <M5AtomDisplay.h>  /// AtomDisplay を使う場合、これを追加する。

#include <M5Unified.h>

  /// この記述で、以後 "M5.Display" の代わりに "dsp" と書くことができるようになる。
auto &dsp = M5.Display;

  /// この記述で、以後 "M5.BtnA" の代わりに "btn" と書くことができるようになる。
  /// 操作対象のボタンを変更したい場合はここの記述を変更する。
auto &btn = M5.BtnA;

  /// CoreInk,Core2,CoreS3,Tough,StickC/CPlus の場合は M5.BtnPWR で電源ボタンも使用できる。
// auto &btn = M5.BtnPWR;
  /// ※ 但しCoreInk以外の機種の BtnPWR については、電源管理ICがボタンを管理しており、
  ///    今ボタンが押された状態かどうかは判定ができない。
  ///    使える機能は wasClick / wasHold および連続クリック回数に関する機能のみに限定される。

  /// この記述で "M5.Speaker" の代わりに "spk" と書くことができるようになる。
auto &spk = M5.Speaker;

void setup(void)
{
  /// 設定用の構造体を取得。
  auto cfg = M5.config();
//cfg.external_speaker.hat_spk = true;    /// HAT SPK  を使う場合、これを追加する。
//cfg.external_speaker.hat_spk2 = true;   /// HAT SPK2 を使う場合、これを追加する。
//cfg.external_speaker.atomic_spk = true; /// ATOM SPK を使う場合、これを追加する。

  /// M5Unifiedを使用する準備をする。
  M5.begin(cfg);

  /// 電子ペーパの場合は描画速度が最も速いモードに変更する。
  dsp.setEpdMode(m5gfx::epd_fastest);

  /// 画面が縦長の場合は横長配置に変更する。
  if (dsp.width() < dsp.height())
  {
    dsp.setRotation(dsp.getRotation() ^ 1);
  }

  /// ボタンの長押し判定の時間 (初期値500) を変更したい場合はこの関数で設定する。
// btn.setHoldThresh(300); // 判定時間を300ミリ秒に変更。

  /// startWriteにはディスプレイに通信バスを占有させ、描画時間を短縮する効果がある。
  /// 電子ペーパやOLEDモデルの場合、dsp.display(); を呼出すまで通信が保留される。
  /// ※ 本来は endWriteと対で使う関数だが、本サンプルでは常時占有したままとする。
  dsp.startWrite();
}

void loop(void)
{
  delay(1);

  /// ボタンの状態を更新する。
  M5.update();

  /// 1回クリック時
  if (btn.wasSingleClicked())
  {
    M5_LOGI("wasSingleClicked");

    /// 音を鳴らす
    spk.tone(500, 500, 0);

    /// 画面の色を黄色にする
    dsp.fillScreen(TFT_YELLOW);
  }

  /// 2回クリック時
  if (btn.wasDoubleClicked())
  {
    M5_LOGI("wasDoubleClicked");

    spk.tone(1500, 500, 0);
    dsp.fillScreen(TFT_BLUE);
  }

  /// 長押し状態になった時
  if (btn.wasHold())
  {
    M5_LOGI("wasHold   getClickCount:%d", btn.getClickCount());

    dsp.fillScreen((btn.getClickCount() & 1) ? TFT_BLACK : TFT_WHITE);
  }

  /// 長押し中
  if (btn.isHolding())
  {
    static int Hz = 1000;

    int click_count = btn.getClickCount();
    /// 長押し直前の単押しの連続回数によって分岐
    if (click_count & 1) // 回数が奇数の場合
    {
      if (Hz > 10) { Hz -= (click_count + 2) >> 1; }  /// 音程を下げる
    }
    else /// 回数が偶数の場合
    {
      if (Hz < 10000) { Hz += (click_count + 2) >> 1;} /// 音程を上げる
    }
    spk.tone(Hz, 500, 0);
  }

  /// ディスプレイが描画待ちになっているか判定する
  /// (電子ペーパの場合ここで描画をスキップすることがある)
  if (!dsp.displayBusy())
  {
    /// 電子ペーパやUnitOLEDの場合はこのタイミングで画面に反映される。
    dsp.display();
  }
}
