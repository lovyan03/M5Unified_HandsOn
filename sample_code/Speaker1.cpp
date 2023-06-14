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

  /// 電子ペーパの場合は描画がはやいモードに変更する。
  M5.Display.setEpdMode(m5gfx::epd_fast);

  /// ボタンの長押し判定の時間 (初期値500) を変更したい場合はこの関数で設定する。
// M5.BtnA.setHoldThresh(300); // 判定時間を300ミリ秒に変更。
// M5.BtnPWR.setHoldThresh(300); // 判定時間を300ミリ秒に変更。

  /// startWriteにはディスプレイに通信バスを占有させ、描画時間を短縮する効果がある。
  /// 電子ペーパやOLEDモデルの場合、dsp.display(); を呼出すまで通信が保留される。
  /// ※ 本来は endWriteと対で使う関数だが、本サンプルでは常時占有したままとする。
  M5.Display.startWrite();

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
    delay(1);
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
  if (step < 0) { step = 6; }
  if (step > 6) { step = 0; }

  /// 画面の表示をクリアする。(電子ペーパの場合は白、それ以外は黒)
  M5.Display.fillScreen(M5.Display.isEPD() ? TFT_WHITE : TFT_BLACK);

  M5.Display.setCursor(0,0);
  M5.Display.printf("step:%d", step);

  uint32_t msec = millis();
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
  case 6: step6(); break;
  }
  msec = millis() - msec;

  M5_LOGI("step:%d   %d msec", step, msec);
}

void step0()
{
  /// 880Hzの音を鳴らす。
  M5.Speaker.tone(880, 400);
  /// 引数1 : 周波数
  /// 引数2 : 音の長さ [ミリ秒]

  /// シリアルモニタに出力される実行時間を確認してみると、
  /// 400ミリ秒の再生にも関わらずstep0の実行時間は一瞬で終了している。
  /// これはtone関数はバックグラウンドで音を再生する仕組みになっているため、
  /// 音の再生が終わるのを待たずに先の処理に進むことができる。
}

void step1()
{
  /// ド・ミ・ソ を同時に鳴らす。
  M5.Speaker.tone(523.251, 400);  // ドの音
  M5.Speaker.tone(659.255, 400);  // ミの音
  M5.Speaker.tone(783.991, 400);  // ソの音

  /// このように連続で呼び出した場合は同時に再生され、和音になる。

  /// 厳密には、最大8音まで重ねる事ができる「仮想チャンネル」という概念があり
  /// 引数3 にて0~7を指定し、8個のうち どのチャンネルを使用するかを設定できるようになっている。
  /// 引数3 を省略または -1 を指定した場合、空いている仮想チャンネルが自動的に割り当てられる。
  /// この例では 引数3を省略しているため、3つのtone関数は別の仮想チャンネルが割当てられている。
}

void step2()
{
  /// ド・ミ・ソ を順に鳴らす。
  M5.Speaker.tone(523.251, 400, 0, true);  // ドの音
  M5.Speaker.tone(659.255, 400, 0, false);  // ミの音
  M5.Speaker.tone(783.991, 400, 0, false);  // ソの音

  /// 引数3 : 仮想チャンネル番号
  /// 引数4 : true=再生中の音を止めて即座に再生を開始する / false=鳴り終わるのを待機する / 省略時はtrue。

  /// 前の例と比べて引数3と4の指定が追加されている。
  /// 仮想チャンネル0番を使用し、前の音が鳴り終わるのを待つ設定となっている。
  /// このため、この3つのtone関数の音は重なったり中断されたりせずに、順番に鳴る。

  /// 各仮想チャンネルには再生を待機できるキューがひとつだけ用意されている。
  /// この例では以下のような状況が発生する。
  /// 1つ目のtone : 即座に再生され、キューには積まれない。
  /// 2つ目のtone : 前のtoneが再生中なので、再生待ちキューに積まれる。
  /// 3つ目のtone : 再生待ちキューが使用中なので、キューが空くまで待機する。
  /// このような状況になるため、この例では実行時間が約400msecほどかかることになる。
}

void step3()
{
  /// ド・ミ・ソ を重ねて順に鳴らす
  M5.Speaker.tone(523.251, 1000, 0);  // ドの音
  delay(200);
  M5.Speaker.tone(659.255, 1000, 1);  // ミの音
  delay(200);
  M5.Speaker.tone(783.991, 1000, 2);  // ソの音

  /// 音を重ねつつ、タイミングをずらして順番に鳴らしたい場合は、待機処理を追加する。
  /// この例では 200ミリ秒ずつタイミングをずらして和音が鳴る。
}

void step4()
{
  /// ド・ミ・ソ を順に、前の音を止めて鳴らす
  M5.Speaker.tone(523.251, 1000, 0);  // ドの音
  delay(200);
  M5.Speaker.tone(659.255, 1000, 0);  // ミの音
  delay(200);
  M5.Speaker.tone(783.991, 1000, 0);  // ソの音

  /// step3では各tone関数が異なる仮想チャンネルを指定していたのに対し、
  /// この例は全て同じ仮想チャンネルを指定している。
  /// 前の音が鳴り終わる前に次のtone関数が実行されるため、
  /// 再生中の音を停止して次の音が鳴り始める。
}

void step5()
{
  /// ピポッ
  M5.Speaker.tone(2000, 100, 0, true);
  M5.Speaker.tone(1000, 100, 0, false);
}

void step6()
{
  /// 音階
  for (int i = 0; i < 60; ++i)
  {
    float Hz = 220 * powf(2.0, i / 12.0f);
    M5.Speaker.tone(Hz, 50, 0, false);
  }
}
