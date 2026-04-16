# Terminal Helper

A simple C utility built to automate and simplify terminal usage.

## Purpose

This project serves two main goals:

1. **Practical Tooling:** A lightweight helper for my day-to-day workflow in the terminal.
2. **Learning Lab:** My personal space for studying pure C and moving away from high-level languages to understand systems programming better.

So basically, I'm re-creating the history tool, as I found out too late it exists :0 

## Usage

### Compilation
To build the binary on Linux or macOS:
```bash
gcc terminal-helper.c -o terminal-helper
```

### Execution
```bash
# Basic search
./terminal-helper -s gcc

# Search with limit (recommended short form)
./terminal-helper -sm10 gcc

# Search with limit (explicit form)
./terminal-helper -s -m 10 gcc
```

For all available flags and examples, see flag-info.md.

## Collaboration
Anyone is welcome to jump in! Whether you've found a bug, a memory leak, or just have a suggestion on how to make the code cleaner or more efficient, feel free to open a Pull Request or start a discussion. I'm always looking to improve the logic and learn from others.

Happy coding! :)
