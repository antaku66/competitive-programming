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

## Build & Run

| Action | Shortcut | Description |
|--------|----------|-------------|
| Build & Run | `Ctrl+Shift+B` | Default task (debug build and run) |
| Build only | Select task | C++: Build |
| Release Build & Run | Select task | C++: Build & Run (Release) |

### Compiler Options

- **Debug**: `-std=gnu++23 -O2 -g -Wall -Wextra`
- **Release**: `-std=gnu++23 -O3 -DNDEBUG -Wall -Wextra`

## Debugging

| Action | Shortcut | Description |
|--------|----------|-------------|
| Start debugging | `F5` | Debug with GDB |
| Toggle breakpoint | `F9` | Add/remove breakpoint on line |
| Step over | `F10` | Go to next line |
| Step into | `F11` | Step into function |

## Directory Structure

```
src/
  main.cpp      # Main source file (template)
.vscode/        # VS Code settings
.clang-format   # Code formatting settings (Google style)
```

## Notes

- Code is automatically formatted on save (clang-format)
- Supports C++23 standard
