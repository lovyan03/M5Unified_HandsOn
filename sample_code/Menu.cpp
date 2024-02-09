// #include <M5UnitRCA.h>      /// UnitRCA を使う場合、これを追加する。
// #include <M5UnitLCD.h>      /// UnitLCD を使う場合、これを追加する。
// #include <M5UnitOLED.h>     /// UnitOLED を使う場合、これを追加する。
// #include <M5UnitGLASS.h>    /// UnitGLASS を使う場合、これを追加する。
// #include <M5AtomDisplay.h>  /// AtomDisplay を使う場合、これを追加する。

#include <M5Unified.h>

//----------------------------------------------------------------

static int menu_x = 2;
static int menu_y = 20;
static int menu_w = 120;
static int menu_h = 30;
static int menu_padding = 36;

static void func_hello()
{
  for (int i = 0; i < 128; ++i)
  {
    M5.Display.drawString("hello", rand() % M5.Display.width(), rand() % M5.Display.height());
  }
}
static void func_rect()
{
  int w = M5.Display.width() >> 3;
  int h = M5.Display.height() >> 3;
  for (int i = 0; i < 128; ++i)
  {
    M5.Display.fillRect( rand() % M5.Display.width()
                       , rand() % M5.Display.height()
                       , w, h
                       , (i&1) ? TFT_BLACK:TFT_WHITE);
  }
}
static void func_line()
{
  for (int i = 0; i < 128; ++i)
  {
    M5.Display.drawLine( rand() % M5.Display.width()
                       , rand() % M5.Display.height()
                       , rand() % M5.Display.width()
                       , rand() % M5.Display.height()
                       , (i&1) ? TFT_BLACK:TFT_WHITE);
  }
}
static void func_circle()
{
  int r = (M5.Display.width() + M5.Display.height()) >> 4;
  for (int i = 0; i < 64; ++i)
  {
    M5.Display.fillCircle( rand() % M5.Display.width()
                         , rand() % M5.Display.height()
                         , r
                         , (i&1) ? TFT_BLACK:TFT_WHITE);
  }
}
static void func_triangle()
{
  for (int i = 0; i < 32; ++i)
  {
    M5.Display.fillTriangle( rand() % M5.Display.width()
                           , rand() % M5.Display.height()
                           , rand() % M5.Display.width()
                           , rand() % M5.Display.height()
                           , rand() % M5.Display.width()
                           , rand() % M5.Display.height()
                           , (i&1) ? TFT_BLACK:TFT_WHITE);
  }
}
static void func_arc()
{
  int r = (M5.Display.width() + M5.Display.height()) >> 4;
  for (int i = 0; i < 64; ++i)
  {
    M5.Display.fillArc( rand() % M5.Display.width()
                      , rand() % M5.Display.height()
                      , r
                      , r >> 1
                      , rand() % 360
                      , rand() % 360
                      , (i&1) ? TFT_BLACK:TFT_WHITE);
  }
}

/// メニュー用の構造体。タイトルの文字列と対応する関数のポインタを持つ
struct menu_item_t
{
  const char* title;
  void (*func)(void);
};

/// メニューの定義
static const menu_item_t menus[] =
{
  { "hello"     , func_hello    },
  { "rect"      , func_rect     },
  { "line"      , func_line     },
  { "circle"    , func_circle   },
  { "triangle"  , func_triangle },
  { "arc"       , func_arc      },
};
/// メニューの要素数
static constexpr const size_t menu_count = sizeof(menus) / sizeof(menus[0]);

/// 現在カーソルのある位置
size_t cursor_index = 0;

void draw_menu(size_t index, bool focus)
{
  auto baseColor = M5.Display.getBaseColor();
  M5.Display.setColor(focus ? baseColor : ~baseColor);
  M5.Display.drawRect(menu_x  , menu_y + index * menu_padding  , menu_w  , menu_h  );
  M5.Display.drawRect(menu_x+1, menu_y + index * menu_padding+1, menu_w-2, menu_h-2);
  M5.Display.setColor(focus ? ~baseColor : baseColor);
  M5.Display.fillRect(menu_x+2, menu_y + index * menu_padding+2, menu_w-4, menu_h-4);
  M5.Display.setTextDatum(textdatum_t::middle_center);
  M5.Display.setTextColor(focus ? baseColor : ~baseColor, focus ? ~baseColor : baseColor);
  M5.Display.drawString(menus[index].title, menu_x + (menu_w >> 1), menu_y + index * menu_padding + (menu_h >> 1));
}

void select_menu(size_t index)
{
  /// 操作音を鳴らす。
  float Hz = 880 * powf(2.0, index / 12.0f);
  M5.Speaker.tone(Hz, 100);
  cursor_index = index;
}

void move_menu(bool back = false)
{
  if (back)
  {
    select_menu((cursor_index ? cursor_index : menu_count) - 1);
  }
  else
  {
    select_menu((cursor_index + 1) % menu_count);
  }
}

void exec_menu(bool holding)
{
  /// holding は長押し中とそれ以外で処理を変えたい場合に利用できる。
  if (holding == false)
  {
    M5.Speaker.tone(880, 150);
  }

  if (menus[cursor_index].func != nullptr)
  {
    M5.Display.setClipRect(menu_x + menu_w, 0, M5.Display.width(), M5.Display.height());
    menus[cursor_index].func();
    M5.Display.clearClipRect();
  }
}


void setup(void)
{
  auto cfg = M5.config();

  M5.begin(cfg);

  if (M5.Display.width() > M5.Display.height())
  {
    M5.Display.setRotation(M5.Display.getRotation() ^ 1);
  }

  /// 外付けディスプレイをメイン画面として使いたい場合はこれらの設定を使う。
/*
  M5.setPrimaryDisplayType( {
      m5::board_t::board_M5ModuleDisplay,
      m5::board_t::board_M5AtomDisplay,
      m5::board_t::board_M5ModuleRCA,
   // m5::board_t::board_M5UnitOLED,
   // m5::board_t::board_M5UnitLCD,
   // m5::board_t::board_M5UnitRCA,
  } );
//*/

  menu_w = M5.Display.width() >> 1;
  if (menu_w < 70)
  {
    menu_y = 10;
    menu_w = (M5.Display.width() * 4) / 5;
  }
  else
  {
    M5.Display.setFont(&fonts::DejaVu18);
    if (M5.Display.width() > 400)
    {
      M5.Display.setTextSize(2);
      menu_y *= 2;
      menu_w = M5.Display.width() >> 1;
    }
  }
  menu_x = 0;

  menu_padding = (M5.Display.height() - menu_y) / menu_count;
  menu_h = menu_padding - 2;

  /// このサンプルでは、startWriteをしたまま、対になるendWriteを使わないようにする。
  M5.Display.startWrite();

  M5.Display.setEpdMode(epd_mode_t::epd_fastest);
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(menu_x, 0);
  M5.Display.print("MENU");
  for (size_t i = 0; i < menu_count; i++)
  {
    draw_menu(i, i == cursor_index);
  }
}

void loop(void)
{
  /// ディスプレイがビジー状態でない場合のみ処理する。
  /// EPDでは画面の更新中はBusyとなるためここを通らない。
  if (!M5.Display.displayBusy())
  { // 選択しているメニューが変更されていれば再描画。
    static size_t prev_index = 0;
    if (prev_index != cursor_index)
    {
      draw_menu(prev_index, false);
      draw_menu(cursor_index, true);
      prev_index = cursor_index;
    }
    /// 表示内容を画面に反映する。
    M5.Display.display();
    /// ※ M5Paper, CoreInk, Unit OLED についてはここで画面が更新される。
  }
  { /// 10ミリ秒間隔で処理が進むように待機する。
    static uint32_t prev_ms;
    uint32_t ms = M5.millis();
    int diff = (10 - (ms - prev_ms));
    if (diff > 0)
    {
      ms += diff;
      M5.delay(diff);
    }
    prev_ms = ms;
  }
  M5.update();

  auto touch_count = M5.Touch.getCount();
  for (size_t i = 0; i < touch_count; i++)
  {
    auto detail = M5.Touch.getDetail(i);
    if ( ((size_t)detail.x - menu_x) < menu_w)
    {
      size_t index = (detail.y - menu_y) / menu_padding;
      if (index < menu_count)
      {
        if (detail.wasPressed())
        {
          select_menu(index);
        }
        else
        if (index == cursor_index)
        {
          if (detail.wasClicked())
          {
            exec_menu(false);
          }
          else
          if (detail.isHolding())
          {
            exec_menu(true);
          }
        }
      }
    }
  }

  switch (M5.getBoard())
  {
  default:
    if (M5.BtnA.wasClicked())   { move_menu(false); }
    if (M5.BtnA.wasHold())      { exec_menu(false); }
    if (M5.BtnA.isHolding())    { exec_menu(true);  }
    break;

  case m5::board_t::board_M5StickC:
  case m5::board_t::board_M5StickCPlus:
    if (M5.BtnPWR.wasClicked()) { move_menu(false); }
    if (M5.BtnB.wasClicked())   { move_menu(true);  }
    if (M5.BtnA.wasClicked())   { exec_menu(false); }
    if (M5.BtnA.wasHold())      { exec_menu(false); }
    if (M5.BtnA.isHolding())    { exec_menu(true);  }
    break;

  case m5::board_t::board_M5Stack:
  case m5::board_t::board_M5StackCore2:
  case m5::board_t::board_M5Tough:
  case m5::board_t::board_M5StackCoreInk:
  case m5::board_t::board_M5Paper:
  case m5::board_t::board_M5Station:
    if (M5.BtnA.wasClicked() || M5.BtnA.wasHold()) { move_menu(true);  }
    if (M5.BtnC.wasClicked() || M5.BtnC.wasHold()) { move_menu(false); }
    if (M5.BtnB.wasClicked())                      { exec_menu(false); }
    if (M5.BtnB.wasHold())                         { exec_menu(false); }
    if (M5.BtnB.isHolding())                       { exec_menu(true);  }
  }
}

#if !defined ( ARDUINO )
extern "C" {
  void loopTask(void*)
  {
    setup();
    for (;;) {
      loop();
    }
    vTaskDelete(NULL);
  }

  void app_main()
  {
    xTaskCreatePinnedToCore(loopTask, "loopTask", 8192, NULL, 1, NULL, 1);
  }
}
#endif
