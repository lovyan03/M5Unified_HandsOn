// #include <M5UnitLCD.h>    // UnitLCD   を使う場合これを追加
// #include <M5UnitOLED.h>   // UnitOLED  を使う場合これを追加
// #include <M5UnitGLASS.h>  // UnitGLASS を使う場合これを追加

#include <M5Unified.h>
void setup()
{
  auto cfg = M5.config();
//cfg.external_speaker.hat_spk = true;    // HAT SPK  を使う場合これを追加
//cfg.external_speaker.hat_spk2 = true;   // HAT SPK2 を使う場合これを追加
//cfg.external_speaker.atomic_spk = true; // ATOM SPK を使う場合これを追加

  M5.begin(cfg);
  M5.Display.println("Hello M5Unified !");
}

void loop()
{
  delay(1);

  M5.update();

  if (M5.BtnA.wasClicked() || M5.BtnPWR.wasClicked() || M5.Touch.getDetail().wasClicked())
  {
    M5.Speaker.tone(1600, 500);
    M5.Log.println("Click !");
    M5.Display.println("Click !");
  }
}
