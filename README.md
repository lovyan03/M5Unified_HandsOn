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

## 最初に試してみるサンプルコード

 - 環境構築ができたら、このリポジトリを各自PCにクローン (ダウンロード) します。
 - クローンしてできたフォルダを `VSCode`から`フォルダを開く`で開きます。
 - `VSCode`の画面左端にあるPlatformIOのアイコンを選択し、`PROJECT TASKS`メニューを開きます。
 - `esp32_rom4m`などの環境が並んでいるので、対象機種にあった環境のツリーを開きます。
    - ESP32 搭載機の場合 `esp32_rom4m`
    - ESP32-S3 搭載機の場合 `esp32s3`
    - ESP32-C3 搭載機の場合 `esp32c3`
 - 環境別のツリーの下にある`General`->`Upload` をクリックします。
 - プログラムのコンパイルが実行され、完了すると実機への書込みが始まります。
 - 実機の画面左上に `Hello M5Unified !` が表示されます。
 - Aボタン または タッチパネルを **短押し** した時に音が鳴り、画面にテキストが表示されます。


---

 - [画面描画のサンプルコード1](sample_code/Display1.cpp)
 - [ボタンのサンプルコード1](sample_code/Button1.cpp)
 - [ボタンのサンプルコード2](sample_code/Button2.cpp)

