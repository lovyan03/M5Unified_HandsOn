// #include <M5UnitRCA.h>      // UnitRCA を使う場合これを追加
// #include <M5UnitLCD.h>      // UnitLCD を使う場合これを追加
// #include <M5UnitOLED.h>     // UnitOLED を使う場合これを追加
// #include <M5UnitGLASS.h>    // UnitGLASS を使う場合これを追加
// #include <M5AtomDisplay.h>  // AtomDisplay を使う場合これを追加

#include <M5Unified.h>

void step();

void setup()
{
  auto cfg = M5.config(); // 設定用の構造体を取得。
//cfg.external_speaker.hat_spk = true;    // HAT SPK  を使う場合これを追加
//cfg.external_speaker.hat_spk2 = true;   // HAT SPK2 を使う場合これを追加
//cfg.external_speaker.atomic_spk = true; // ATOM SPK を使う場合これを追加

// 最初にbeginを実行します。M5Unifiedの準備に必ず必要です。
  M5.begin(cfg);

  // 実験用の関数を実行する。
  step();
}

void loop(void)
{
  delay(1);
  M5.update();

  // ボタンA,ボタンB,電源ボタン,画面のいずれかをクリックする度に実験用の関数を実行する。
  if (M5.BtnA.wasClicked() || M5.BtnB.wasClicked() || M5.BtnPWR.wasClicked() || M5.Touch.getDetail().wasClicked())
  {
    step();
  }
}

// 実験用の関数群
void step1();
void step2();
void step3();
void step4();
void step5();
void step6();

void step()
{
  static int step = 1;

  M5_LOGI("step:%d", step);
  // 操作音を鳴らす。
  M5.Speaker.tone(1000, 30);

  // 画面描画を高速化するおまじない
  M5.Display.startWrite();

  // 画面の表示をクリアする。(電子ペーパの場合は白、それ以外は黒)
  M5.Display.clear();
  M5.Display.setCursor(0, 0);

  M5.Display.printf("step:%d ", step);

  // 実験用の関数を順番に試す。
  switch (step)
  {
  default: step = 1;
  case 1: step1(); break;
  case 2: step2(); break;
  case 3: step3(); break;
  case 4: step4(); break;
  case 5: step5(); break;
  case 6: step6(); break;
  }

  M5_LOGI("done.");
  M5.Display.println(": done");

  // startWriteと対でendWriteを使う。
  // UnitOLEDや電子ペーパーの場合はここで画面に反映される。
  M5.Display.endWrite();

  step += 1;
}

void step1()
{
// ド・ミ・ソ を順に鳴らす
  M5.Speaker.tone(523.251, 400);  // ドの音
  delay(400);
  M5.Speaker.tone(659.255, 400);  // ミの音
  delay(400);
  M5.Speaker.tone(783.991, 400);  // ソの音

// tone関数の引数1が周波数、引数2が音の長さをミリ秒で指定します。
// toneにて400ミリ秒鳴らし delay関数で同じく400ミリ秒待機させるので、
// 結果として３つの音は順番に鳴ります。
}

void step2()
{
// ド・ミ・ソ を重ねて鳴らす
  M5.Speaker.tone(523.251, 1000);  // ドの音
  delay(400);
  M5.Speaker.tone(659.255, 1000);  // ミの音
  delay(400);
  M5.Speaker.tone(783.991, 1000);  // ソの音

// step1と比べて、各tone関数の引数2が1000ミリ秒に増えています。
// 一方、delay関数の待機時間は400ミリ秒のままです。
// つまり前の音がまだ鳴っている時に次のtone関数が実行されます。
// この場合は、音が合成されて鳴ります。

// M5UnifiedのSpeakerには仮想的な8個のチャンネルがあり、同時に8音まで重ねて出力できます。
// どのチャンネルを使用するかはtone関数の引数3にて、 0~7から選択できます。
// 省略時や -1 を指定した場合は、未使用のチャンネルから自動的に選ばれます。
// 上記の例では引数3は省略されているため、未使用チャンネルが割当てられて、重ねて出力されます。
}

void step3()
{
// ド・ミ・ソ を順に、前の音を止めて鳴らす
  M5.Speaker.tone(523.251, 1000, 0);  // ドの音
  delay(400);
  M5.Speaker.tone(659.255, 1000, 0);  // ミの音
  delay(400);
  M5.Speaker.tone(783.991, 1000, 0);  // ソの音

// step2と比べて、各tone関数の引数3に0の指定が追加されています。
// この場合、各tone関数はすべて同じチャンネル0番を使用します。
// 前の音が鳴り終わる前に次のtone関数が実行されます。
// この場合の動作は、再生中の音が停止され、次の音が鳴り始めます。
// 具体的には、ドの音が1000ミリ秒鳴らす予定だったところを、
// 400ミリ秒経過した時点で次のミの音の再生指示が行われるわけです。
// チャンネル0番でミの音を鳴らそうとしますが、このチャンネルはドの音がまだ鳴っています。
// このとき、ドの音の出力は中断されて、即座にミの音が鳴り始めます。

// 一番最初のサンプル (400ミリ秒鳴らして400ミリ秒delayする)のと比べて、
// 音の切り替わりが滑らかに繋がっていることを聞き比べてみてください。
}

void step4()
{
// ド・ミ・ソ を順に、前の音が鳴り終わるのを待ってから鳴らす
  M5.Speaker.tone(523.251, 1000, 0, false);  // ドの音
  M5.Speaker.tone(659.255, 1000, 0, false);  // ミの音
  M5.Speaker.tone(783.991, 1000, 0, false);  // ソの音

// step3と比べて、各tone関数の引数4にfalseの指定が追加されています。
// 引数4は、今鳴っている音の扱いを選択できます。「true=即座に停止」「false=鳴り終わりを待機」
// 省略時はtrueです。前のサンプルでは 引数4は省略され true が指定されたのと同じ状態だったのです。
// また、各tone関数の間にあった delay がありません。

// 一番最初のサンプル (400ミリ秒鳴らして400ミリ秒delayする)のと比べて、
// 音の切り替わりが滑らかに繋がっていることを聞き比べてみてください。
}

void step5()
{
}

void step6()
{
}
