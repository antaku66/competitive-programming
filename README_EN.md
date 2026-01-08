# competitive-programming

A C++ development environment for competitive programming.

## Prerequisites

The following software must be installed.

### MSYS2 (ucrt64)

The following packages are required:

- GCC (g++)
- GDB
- clang-format

Add `C:/msys64/ucrt64/bin` to your PATH.

### Visual Studio Code

Install the following extension:

- [C/C++](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools) (ms-vscode.cpptools)

## Build

```bash
g++ -std=gnu++23 -O2 -Wall -Wextra src/main.cpp -o main.exe
```

## Directory Structure

```text
src/
  main.cpp      # Main source file (template)
  header.hpp    # Common header (macros and type definitions)
  algorithm/    # Algorithm library
  heuristic/    # Heuristic library
.vscode/        # VS Code settings
.clang-format   # Code formatting settings (Google style)
```

## Notes

- Code is automatically formatted on save (clang-format)
- Supports C++23 standard
