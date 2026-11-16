[日本語版は下にあります](#日本語)

## PonPon (work in progress)

PonPon is a small single player riichi mahjong game.

It's mostly an experiment to learn a bit of C and make games without using an engine. Although I'm using raylib, the rest is all made by me.

Currently way incomplete, but I'm slowly advancing.

### How to run

Requires MSYS2 UCRT64 toolchain (uses UCRT64's `clang`)

1. Modify `MSYS2_ROOT` in `vars.bat.example` to the root directory of your MSYS2
2. Rename `vars.bat.example` to `vars.bat`
3. Run `build.bat`
4. Run the generated executable `bin/ponpon.exe`. You'll need to run from the root folder because the linked executable looks for the assets in the `assets` folder.

---

## 日本語

### PonPon（開発中）

PonPonは、ひとりで遊べる小さなリーチ麻雀ゲームです。

C言語を少し勉強しながら、ゲームエンジンを使わずにゲームを作ってみるための実験です。raylibを使っていますが、それ以外は自分で作っています。

まだ作りかけですが、少しずつ開発を進めています。

#### ビルドと起動

MSYS2のUCRT64ツールチェーンが必要です。ビルドにはUCRT64版の `clang` を使います。

1. `vars.bat.example`の`MSYS2_ROOT`に、MSYS2のインストール先のルートディレクトリを指定します。
2. `vars.bat.example`を`vars.bat`にリネームします。
3. `build.bat`を実行します。
4. 生成された `bin/ponpon.exe` をリポジトリのルートディレクトリから実行します。ゲームは作業ディレクトリ内の`assets`フォルダーを参照しますので必ずリポジトリのルートから実行してください。
