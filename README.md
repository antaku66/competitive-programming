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

## ビルド・実行方法

| 操作 | ショートカット | 説明 |
|------|---------------|------|
| ビルド＆実行 | `Ctrl+Shift+B` | デフォルトタスク（デバッグビルド後に実行） |
| ビルドのみ | タスク選択 | C++: Build |
| リリースビルド＆実行 | タスク選択 | C++: Build & Run (Release) |

### コンパイルオプション

- **デバッグ**: `-std=gnu++23 -O2 -g -Wall -Wextra`
- **リリース**: `-std=gnu++23 -O3 -DNDEBUG -Wall -Wextra`

## ディレクトリ構成

```
src/
  main.cpp      # メインソースファイル（テンプレート）
.vscode/        # VS Code 設定
.clang-format   # コードフォーマット設定（Google スタイル）
```

## 補足

- ファイル保存時に自動でコードフォーマットが適用されます（clang-format）
- C++23 標準に対応しています
