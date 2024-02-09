// #include <M5UnitRCA.h>      /// UnitRCA を使う場合、これを追加する。
// #include <M5UnitLCD.h>      /// UnitLCD を使う場合、これを追加する。
// #include <M5UnitOLED.h>     /// UnitOLED を使う場合、これを追加する。
// #include <M5UnitGLASS.h>    /// UnitGLASS を使う場合、これを追加する。
// #include <M5AtomDisplay.h>  /// AtomDisplay を使う場合、これを追加する。

#include <M5Unified.h>


int max_y[64];
int prev_y[64];
uint16_t colors[64];

uint16_t rainbow(int value)
{
  uint8_t red = 0;
  uint8_t green = 0;
  uint8_t blue = 0;

  uint8_t quadrant = value >> 5;

  if (quadrant == 0) {
    blue = 31;
    green = 2 * (value & 31);
    red = 0;
  }
  if (quadrant == 1) {
    blue = 31 - (value & 31);
    green = 63;
    red = 0;
  }
  if (quadrant == 2) {
    blue = 0;
    green = 63;
    red = value & 31;
  }
  if (quadrant == 3) {
    blue = 0;
    green = 63 - 2 * (value & 31);
    red = 31;
  }
  return (red << 11) + (green << 5) + blue;
}

void setup(void)
{
  /// M5Unifiedを使用する準備をする。
  M5.begin();

  /// 電子ペーパの場合は描画速度が最も速いモードに変更する。
  M5.Display.setEpdMode(m5gfx::epd_fastest);

  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setColorDepth(16);

  /// 画面が縦長の場合は横長配置に変更する。
  if (M5.Display.width() < M5.Display.height())
  {
    M5.Display.setRotation(M5.Display.getRotation() ^ 1);
  }

  memset(prev_y, 0x7F, 64 * sizeof(int));
  memset(max_y, 0x7F, 64 * sizeof(int));
  for (int x = 0; x < 64; ++x)
  {
    colors[x] = rainbow(x*2);
  }

  /// startWriteにはディスプレイに通信バスを占有させ、描画時間を短縮する効果がある。
  /// 電子ペーパやOLEDモデルの場合、M5.Display.display(); を呼出すまで通信が保留される。
  /// ※ 本来は endWriteと対で使う関数だが、本サンプルでは常時占有したままとする。
  M5.Display.startWrite();
}

int i;

void loop(void)
{
  int h = M5.Display.height();
  if ((++i & 255) == 0)
  {
    M5.delay(1);
  }

  for (int x = 0; x < 64; ++x)
  {
    int y = (h>>1) - (sinf((float)((x-24)*i) / 543.0f) + sinf((float)((x-40)*i) / 210.0f)) * (h>>2);

    int xpos = x * M5.Display.width() / 64;
    int w = ((x+1) * M5.Display.width() / 64) - xpos - 1;
    if (max_y[x]+1 >= y) { max_y[x] = y-1; }
    else
    {
      M5.Display.fillRect(xpos, max_y[x]-3, w, 1, TFT_BLACK);
      max_y[x]++;
    }
    M5.Display.fillRect(xpos, max_y[x]-3, w, 3, TFT_WHITE);

    if (prev_y[x] < y)
    {
      M5.Display.fillRect(xpos, prev_y[x], w, y - prev_y[x], TFT_BLACK);
    }
    else
    {
      M5.Display.fillRect(xpos, y, w, prev_y[x] - y, colors[x]);
    }
    // M5.Display.setCursor(xpos, h-8);
    // M5.Display.printf("%03d", h-y);
    prev_y[x] = y;
  }

  if (!M5.Display.displayBusy())
  { /// 電子ペーパやOLEDの画面更新処理
    M5.Display.display();
  }
}
