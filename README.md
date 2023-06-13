# M5Unifiedハンズオン

このリポジトリは`M5Unified`を実際に使って試す、ハンズオン イベント用の資料、サンプルコードの公開用です。

## M5Unifiedハンズオンの準備

進行用のスライドは[こちら](https://docs.google.com/presentation/d/1a_PSTQC-5HFejekAp_ExxMcwR8apyax_pxPR6dsnbnY/) にあります。<br>
https://docs.google.com/presentation/d/1a_PSTQC-5HFejekAp_ExxMcwR8apyax_pxPR6dsnbnY/

---

## 環境構築

[こちらの手順](https://docs.m5stack.com/ja/quick_start/m5unified/intro_vscode)に従い、`Visual Studio Code` + `PlatformIO` で `M5Unified` を使用できる状態にします。<br>
https://docs.m5stack.com/ja/quick_start/m5unified/intro_vscode

サンプルプログラムが実機に書込んで動く状態が出来たら、環境構築は完了です。

---

## ハンズオン用サンプルを試してみよう

 - 環境構築ができたら、このリポジトリを各自PCにクローン (ダウンロード) します。
 - クローンしてできたフォルダを `VSCode`から`フォルダを開く`で開きます。
 - `VSCode`の画面左端にあるPlatformIOのアイコンを選択し、`PROJECT TASKS`メニューを開きます。
 - `esp32_rom4m`などの環境が並んでいるので、対象機種にあった環境のツリーを開きます。
    - ESP32-S3 の場合・・・ `esp32s3`
    - ESP32-C3 の場合・・・ `esp32c3`
    - 普通のESP32 の場合・・ `esp32_rom4m`
 - 開いたツリーの中から `General`->`Upload` をクリックします。
 - プログラムのコンパイルが実行され、完了すると実機への書込みが始まります。

### 正常に動作した場合
 - 実機の画面左上に `Hello M5Unified !` が表示されます。
 - Aボタン または タッチパネルを **短押し** した時に音が鳴り、画面にテキストが表示されます。

---

## 他のサンプルを試すには

このリポジトリの [sample_code](sample_code/) フォルダに、他のサンプルコードをいくつか用意しています。

 - 試したいファイルを開き、中のソースコードを全選択・クリップボードにコピー。
 - src フォルダの main.cpp を開き、全選択・貼り付けしてまるごと上書きする。

ファイルが増えたり名前が変わった場合、ビルド時間が大幅に伸びるので、このようにします。<br>
main.cpp を書き換えただけなら、再ビルド対象が main.cpp のみで済み、時間を短縮できます。

 - [ボタンのサンプルコード1](sample_code/Button1.cpp)
 - [ボタンのサンプルコード2](sample_code/Button2.cpp)
 - [画面描画のサンプルコード1](sample_code/Display1.cpp)
 - [画面描画のサンプルコード2](sample_code/Display2.cpp)
 - [スピーカーのサンプルコード1](sample_code/Speaker1.cpp)

