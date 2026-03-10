# ESP32用espress_stream

<div style="text-align:right">2026年3月11日</div>

## 概要

[espress_stream](http://akino-yo.f5.si/akinoyonika/espress_stream/)の[ESP32](https://www.espressif.com/en/products/socs/esp32)上への移植を行いました。
linux用とは以下のような違いがあります。

- シングルタスク、スレッド未使用。
- リソース固定、ヒープ未使用。
- 排他制御、同期機構関連部のダミー実装による無効化もしくは疑似動作化。
- SDKのライブラリーによるデータグラム通信部。
- SDKのライブラリーによるティック生成部。
- SDKのライブラリーによる乱数生成部。

その他、小リソース向けに設定値を変更しています。

なお、開発環境には[Arduino IDE](https://www.arduino.cc/en/software/#ide) + [ESP32 Arduino Core](https://github.com/espressif/arduino-esp32)を用いました。

本ソフトウェアは[ESP32-S3](https://www.espressif.com/en/products/socs/esp32-s3)を搭載した[Unit CamS3-5MP](https://docs.m5stack.com/en/unit/Unit-CAMS3%205MP)を使って作成されています。
ESP32共通の機能のみを用いて作成しており、ソースにより提供していますので、ほとんどのESP32シリーズを搭載したボードで動作可能と思います。

## パッケージ

本ソフトウェアはソースパッケージにて提供しています。以下をダウンロードしてください。

- [espress_stream_for_esp32.20260311.tar.gz](http://akino-yo.f5.si/akinoyonika/espress_stream_for_esp32/espress_stream_for_esp32.20260311.tar.gz)

<details><summary>変更履歴</summary>

espress_stream_for_esp32.20260311.tar.gz (1.12-1)
: <ul>
    <li>ドキュメントの加筆、修正。</li>
  </ul>

espress_stream_for_esp32.20251001.tar.gz (1.12-1)
: <ul>
    <li>espress_streamのバージョンアップに対応した。</li>
    <li>ソースファイル等の行末文字をLFへの統一。</li>
  </ul>

espress_stream_for_esp32.20250928.tar.gz (1.11-1)
: <ul>
    <li>ドキュメントの修正。</li>
  </ul>

espress_stream_for_esp32.20250925.tar.gz (1.11-1)
: <ul>
    <li>初版。</li>
  </ul>

</details>

## 導入

### 準備

本ソフトウェアはArduino IDEを用いてソースコードから作成します。
Arduino IDE、ESP32用ライブラリーの導入やボード設定等はボードのマニュアル等を参照して行っておいてください。

任意のディレクトリでパッケージの解凍を行っておきます。

```
$ tar zxvf espress_stream_for_esp32.########.tar.gz
```

パッケージには移植されたespress_streamとサンプルアプリケーションが含まれています。
サンプルアプリケーションは[linux用の<tt>es_ccat</tt>と<tt>es_ftx</tt>](http://akino-yo.f5.si/akinoyonika/espress_stream/#%E3%82%B5%E3%83%B3%E3%83%97%E3%83%AB%E3%82%A2%E3%83%97%E3%83%AA%E3%82%B1%E3%83%BC%E3%82%B7%E3%83%A7%E3%83%B3)を移植したものです。
<tt>es_ccat</tt>はクライアントアプリケーションで標準入力からの入力を接続したサーバーアプリケーションに送信し、
サーバーアプリケーションからの受信を標準出力に出力します。
<tt>es_ftx</tt>はクライアントアプリケーションで、サーバーアプリケーション<tt>es_frx</tt>に対してファイルを送信する機能を持ちます。
移植された<tt>es_ftx</tt>は<tt>test.dat</tt>というファイル名で1MBのダミーデータを送信するように作られています。

パッケージの解凍を行うと`espress_stream_for_esp32`というディレクトリができます。
この中にスケッチブックとライブラリーがありますので、これらをArduino IDEに設定した"スケッチブックの場所"に移動するか、
`espress_stream_for_esp32`を"スケッチブックの場所"に設定してください。

以下ではIDEの導入とパッケージの配置が完了していることを前提に、サンプルアプリケーションを動作させるためのバイナリーを作成するまでの手順を示します。

### バイナリーの作成

以下は<tt>es_ftx</tt>におけるバイナリー作成手順を示しています。<tt>es_ccat</tt>については適宜<tt>es_ftx</tt>部分を<tt>es_ccat</tt>に読み替えてください。

1. Arduino IDEのメニューで[ファイル] → [スケッチブック] → [es_ftx]を選択します。
2. 右上の[&hellip;] → [新しいタブ]で<tt>wifi_config.h</tt>を作成し、編集する。
3. <tt>ex_ccat_main.c</tt>タブを選択しこれを編集する。
4. 左上の[✓]ボタン([検証]ボタン)を押すとバイナリーが作成されます。エラーが出ないことを確認してください。

<tt>wifi_config.h</tt>ではwifiアクセスポイントの設定を行っています。

```
#define WIFI_SSID "MY_WIFI_SSID"
#define WIFI_PASSWORD "my_wifi_password"
```

以上のような内容で作成してください。

<tt>es_ftx_main.c</tt>ではサーバーのアドレス、ポート、公開鍵およびユーザーID、パスワードを設定していますので、この部分を編集します。

```
#define ADDRESS "192.168.1.2"
#define PORT "12345"
#define PUBLIC_KEY { \
	0x73, 0x35, 0xe6, 0x3a, 0x76, 0x21, 0x23, 0xa9, 0x41, 0xa2, 0x5e, 0x5e, 0x53, 0x8d, 0xd4, 0xd2, \
	0xe4, 0x7d, 0x86, 0x92, 0x80, 0xf8, 0x3a, 0xf7, 0x81, 0x0e, 0x0b, 0x3d, 0xae, 0x80, 0x69, 0xc9 \
}
#define USER_ID "foo"
#define USER_PASSWORD "tpfBTht8"
```

公開鍵、ユーザーID、パスワードはサーバーの設定に合わせる必要があります。詳細はlinux用パッケージの該当ドキュメントを参照してください。
(<tt>ex_ccat</tt>の場合、ユーザーIDとパスワードの設定はありません。)

バイナリーの作成でエラーが出ないことを確認できたなら、左上の[→]ボタン([書き込み]ボタン)を押してボードにソフトウェアを導入してください。

## 使用方法

### サンプルアプリケーション

#### <tt>es_ccat</tt>

linuxにてサーバーアプリケーション<tt>es_scat</tt>を起動してください。
また、ESP32の起動後にシリアルコンソールが接続するように準備しておいてください。(行末はLFで。)
30秒以内にソフトウェア導入済みのESP32を起動すれば、少し時間をおいて"CONNECT"の表示がコンソールに出ます。
この状態でシリアルコンソールから文字列を入力するとlinux側にその文字列が出力されます。
同様にlinux側で文字列を入力するとシリアルコンソール側に文字列が出力されます。

#### <tt>es_ftx</tt>

linuxにてサーバーアプリケーション<tt>es_frx</tt>を起動、ファイル受信の準備をしておいてください。
ソフトウェア導入済みのESP32を起動すればファイルの転送を開始します。
このとき、シリアルコンソールを接続しておくと、ファイル転送プログレス等の情報が表示されます。

### espress_stream使用アプリケーション作成

<tt>libraries/espress_stream_for_esp32</tt>ディレクトリ以下をディレクトリごと"スケッチブックの場所"で示されるディレクトリにコピーしてください。

新たなスケッチを作成したら、メニューで[スケッチ] → [ライブラリをインクルード]で開くメニュー、
下のほうに<tt>espress_stream for esp32</tt>がありますので、これを選択してください。
"<スケッチ名>.ino"タブで表示されているソースファイルの先頭に<tt>#include <espress_stream_for_esp32.h></tt>行が挿入されます。

アプリケーションからespress_streamを利用するためのAPIはlinux用ライブラリーと同じです。
詳細は「[linux用のライブラリー使用方法](http://akino-yo.f5.si/akinoyonika/espress_stream/#%E4%BD%BF%E7%94%A8%E6%96%B9%E6%B3%95)」を参照してください。

## ライセンス

MITライセンスに基づいてリリースされています。

<details><summary>Copyright 2025 akinoyonika</summary>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the “Software”), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED “AS IS”, WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

</details>
