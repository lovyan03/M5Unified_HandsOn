// #include <M5UnitRCA.h>      /// UnitRCA を使う場合、これを追加する。
// #include <M5UnitLCD.h>      /// UnitLCD を使う場合、これを追加する。
// #include <M5UnitOLED.h>     /// UnitOLED を使う場合、これを追加する。
// #include <M5UnitGLASS.h>    /// UnitGLASS を使う場合、これを追加する。
// #include <M5AtomDisplay.h>  /// AtomDisplay を使う場合、これを追加する。

#include <M5Unified.h>

void setup(void)
{
  /// M5Unifiedを使用する準備をする。
  M5.begin();

  /// 電子ペーパの場合は描画速度が最も速いモードに変更する。
  M5.Display.setEpdMode(m5gfx::epd_fastest);

  /// 画面が縦長の場合は横長配置に変更する。
  if (M5.Display.width() < M5.Display.height())
  {
    M5.Display.setRotation(M5.Display.getRotation() ^ 1);
  }

  M5.Display.setFont(&fonts::AsciiFont8x16);
  M5.Display.setTextSize(M5.Display.height() / 40.0f);
  M5.Display.setTextWrap(false);


  /// ボタンの長押し判定の時間 (初期値500) を変更したい場合はこの関数で設定する。
// btn.setHoldThresh(300); // 判定時間を300ミリ秒に変更。

  /// startWriteにはディスプレイに通信バスを占有させ、描画時間を短縮する効果がある。
  /// 電子ペーパやOLEDモデルの場合、M5.Display.display(); を呼出すまで通信が保留される。
  /// ※ 本来は endWriteと対で使う関数だが、本サンプルでは常時占有したままとする。
  M5.Display.startWrite();
}

/// ユーザーの操作内容を表す定数
enum input_t
{
  input_none,
  input_next,     // 曲送り
  input_prev,     // 曲戻り
  input_vol_up,   // ボリューム増加
  input_vol_down, // ボリューム低下
};

/// ユーザの操作内容を取得する自作関数
input_t getInput(void)
{
  /// ボタンAしかないパターンを想定

  if (M5.BtnA.wasSingleClicked() || M5.BtnPWR.wasSingleClicked())
  { /// 1回クリックで曲送り
    return input_next;
  }

  if (M5.BtnA.wasDoubleClicked() || M5.BtnPWR.wasDoubleClicked())
  { /// 2回クリックで曲戻り
    return input_prev;
  }

  if (M5.BtnA.isHolding())
  { /// 長押し時はボリューム操作
    if (M5.BtnA.getClickCount())
    { // 長押し前にボタンクリック回数がある場合はボリュームを下げる
      return input_vol_down;
    }
    return input_vol_up;
  }
  return input_none;
}

/// 曲の数 (20個あるという想定)
static constexpr int data_count = 20;

/// 現在の曲番号
static int data_index = 0;

/// 現在のボリューム
static int volume = 128;

void loop(void)
{
  M5.delay(10);

  /// ボタンの状態を更新する。
  M5.update();

  switch (getInput())
  {
  case input_next:
    if (++data_index >= data_count) {
      data_index = 0;
    }
    break;

  case input_prev:
    if (--data_index < 0) {
      data_index = data_count - 1;
    }
    break;

  case input_vol_up:
    if (volume < 255) ++volume;
    break;

  case input_vol_down:
    if (volume > 0) --volume;
    break;
  }

  {
    /// 前回のボリューム
    static int prev_volume = -1;

    /// 前回の曲番号
    static int prev_index = -1;

    if (prev_volume != volume)
    { /// ボリュームに変更があった場合の処理
      M5_LOGI("vl:%d", volume);
      M5.Display.setCursor(0, 0);
      M5.Display.printf("vl:%d  ", volume);
      prev_volume = volume;
    }

    if (prev_index != data_index)
    { /// 曲番号に変更があった場合の処理
      M5_LOGI("No:%d", data_index);
      M5.Display.setCursor(0, M5.Display.height() - M5.Display.fontHeight());
      M5.Display.printf("No:%d  ", data_index);
      prev_index = data_index;
    }
  }

  if (!M5.Display.displayBusy())
  { /// 電子ペーパやOLEDの画面更新処理
    M5.Display.display();
  }
}
