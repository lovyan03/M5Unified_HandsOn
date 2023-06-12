// #include <M5UnitRCA.h>      // UnitRCA を使う場合これを追加
// #include <M5UnitLCD.h>      // UnitLCD を使う場合これを追加
// #include <M5UnitOLED.h>     // UnitOLED を使う場合これを追加
// #include <M5UnitGLASS.h>    // UnitGLASS を使う場合これを追加
// #include <M5AtomDisplay.h>  // AtomDisplay を使う場合これを追加

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

void setup(void)
{
  auto cfg = M5.config(); // 設定用の構造体を取得。
//cfg.external_speaker.hat_spk = true;    // HAT SPK  を使う場合これを追加
//cfg.external_speaker.hat_spk2 = true;   // HAT SPK2 を使う場合これを追加
//cfg.external_speaker.atomic_spk = true; // ATOM SPK を使う場合これを追加

  /// 最初にbeginを実行します。M5Unifiedの準備に必ず必要です。
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
  delay(50);

  /// ボタンの状態を更新する。
  M5.update();

  /// ボタンの状態取得関数の結果をdata配列にまとめる。
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

  /// 反応があった時・無かった時の表示色を設定。
  /// 電子ペーパの時とそれ以外で色を分ける。
  int colors[] = {
    dsp.isEPD() ? TFT_WHITE : TFT_BLACK,
    dsp.isEPD() ? TFT_BLACK : TFT_WHITE,
  };

  static int x;
  int y = 0;
  int h = dsp.height() / sizeof(data);
  int w = (dsp.width() > 320) ? 3 : 2;

  int Hz = 440;
  int ch2 = 0;
  for (auto d: data)
  {
    /// 画面に出力する。
    dsp.fillRect(x, y, w, h - 1, colors[d]);
    y += h;
    /// ログ(シリアルモニタ)にも出力する。(改行を送信するまで出力は保留される。)
    M5.Log.printf("%d ", d);

/*  /// 操作音を鳴らして見たい場合はここをコメント解除
    if (d) { M5.Speaker.tone(Hz, 60, ch2 >> 1); }
    Hz = (Hz * 12) / 8;
    ch2 ++;
//*/
  }
  /// 描画位置を右に移動、画面端に達したら左端に戻る。
  if ((x += w) >= dsp.width()) { x = 0; }

  /// ディスプレイが画面更新待ちになっているか判定する
  /// (電子ペーパーの場合ここで描画をスキップすることがある)
  if (!dsp.displayBusy())
  {
    /// 新しい位置に縦線を描画する。
    dsp.fillRect(x, 0, w, h * sizeof(data), TFT_YELLOW);

    /// 電子ペーパやUnitOLEDの場合はこのタイミングで画面に反映される。
    dsp.display();
  }

  /// ログ(シリアルモニタ)にクリック回数と改行を出力する。
  /// ここの末尾で改行 \n を送信するので、これまでの内容が１行にまとめて出力される。
  M5.Log.printf(" / %d \n", btn.getClickCount());
}
