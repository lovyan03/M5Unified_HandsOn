// #include <M5UnitRCA.h>      /// UnitRCA を使う場合、これを追加する。
// #include <M5UnitLCD.h>      /// UnitLCD を使う場合、これを追加する。
// #include <M5UnitOLED.h>     /// UnitOLED を使う場合、これを追加する。
// #include <M5UnitGLASS.h>    /// UnitGLASS を使う場合、これを追加する。
// #include <M5AtomDisplay.h>  /// AtomDisplay を使う場合、これを追加する。

#include <M5Unified.h>

void setup(void)
{
  M5.begin();

  /// もし画面にもログを表示したい場合はこれを記述します。
  /// 複数の画面がある場合、何個目の画面に表示するか引数で指定できます。
  M5.setLogDisplayIndex(0);

  /// 上記の方法のほかに「UnitOLEDにログを出したい」と思った場合は以下のように記述できます。
// M5.setLogDisplayType(m5gfx::board_M5UnitOLED);

  /// ログが画面の末尾に達した際に先頭に戻したい場合はこれを記述します。
  M5.Display.setTextWrap(true, true);

/// 画面の末尾に達した際にスクロールさせたい場合はこれを記述します。
// M5.Display.setTextScroll(true);


/// ログの出力レベルを個別に設定できます。
/// シリアル出力のログレベルを VERBOSEに設定
  M5.Log.setLogLevel(m5::log_target_serial, ESP_LOG_VERBOSE);

/// 画面に表示するログレベル DEBUGに設定
  M5.Log.setLogLevel(m5::log_target_display, ESP_LOG_DEBUG);

/// ログをカラー表示にするか否かを設定できます。
  M5.Log.setEnableColor(m5::log_target_serial, true);
  M5.Log.setEnableColor(m5::log_target_display, true);

/// M5.Log.printf()関数を使用することで、任意の文字列をログ出力できます。
  M5.Log.printf("M5.Log.printf non level output\n");

/// M5.Log() 関数にて、引数でログレベルを指定して出力できます。
  M5.Log(ESP_LOG_ERROR   , "M5.Log error log");    /// ERROR level output
  M5.Log(ESP_LOG_WARN    , "M5.Log warn log");     /// WARN level output
  M5.Log(ESP_LOG_INFO    , "M5.Log info log");     /// INFO level output
  M5.Log(ESP_LOG_DEBUG   , "M5.Log debug log");    /// DEBUG level output
  M5.Log(ESP_LOG_VERBOSE , "M5.Log verbose log");  /// VERBOSE level output

/// M5_LOGx マクロを使用することもできます。こちらはログレベルごとにマクロが用意されているので記述が短くできます。
/// こちらでログ出力した場合は、ログ文字列として時間情報やソースコードのファイル名と行番号情報が追加されます。
  M5_LOGE("M5_LOGE error log");     /// ERROR level output with source info
  M5_LOGW("M5_LOGW warn log");      /// WARN level output with source info
  M5_LOGI("M5_LOGI info log");      /// INFO level output with source info
  M5_LOGD("M5_LOGD debug log");     /// DEBUG level output with source info
  M5_LOGV("M5_LOGV verbose log");   /// VERBOSE level output with source info
}

void loop(void)
{
  M5.delay(1);
  M5.update();

  if (M5.BtnPWR.wasClicked()) { M5_LOGE("BtnP %d click", M5.BtnPWR.getClickCount()); }
  if (M5.BtnA  .wasClicked()) { M5_LOGW("BtnA %d click", M5.BtnA  .getClickCount()); }
  if (M5.BtnB  .wasClicked()) { M5_LOGI("BtnB %d click", M5.BtnB  .getClickCount()); }
  if (M5.BtnC  .wasClicked()) { M5_LOGD("BtnC %d click", M5.BtnC  .getClickCount()); }

  static uint32_t counter = 0;
  if ((++counter & 0x3FF) == 0)
  {
    static int prev_y;
    int cursor_y = M5.Display.getCursorY();
    if (prev_y > cursor_y)
    {
      M5.Display.clear();
    }
    prev_y = cursor_y;
    M5_LOGV("count:%d", counter >> 10);  
  }
}
