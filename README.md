# competitive-programming

C++ 競技プログラミング用の開発環境です。

## 前提条件

以下のソフトウェアがインストールされている必要があります。

### MSYS2 (ucrt64)

以下のパッケージが必要です：

- GCC (g++)
- GDB
- clang-format

PATH に `C:/msys64/ucrt64/bin` を追加してください。

### Visual Studio Code

以下の拡張機能をインストールしてください：

- [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) (ms-vscode.cpptools)

## ビルド方法

```bash
g++ -std=gnu++23 -O2 -Wall -Wextra src/main.cpp -o main.exe
```

## ディレクトリ構成

```text
src/
  main.cpp      # メインソースファイル（テンプレート）
  header.hpp    # 共通ヘッダ（マクロ・型定義）
  algorithm/    # アルゴリズム用ライブラリ
  heuristic/    # ヒューリスティック用ライブラリ
.vscode/        # VS Code 設定
.clang-format   # コードフォーマット設定（Google スタイル）
```

## 補足

- ファイル保存時に自動でコードフォーマットが適用されます（clang-format）
- C++23 標準に対応しています
