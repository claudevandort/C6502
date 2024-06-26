# C6502

## Overview

This is a software emulator for the [6502](https://en.wikipedia.org/wiki/MOS_Technology_6502) microprocessor, written in good old [C99](https://en.wikipedia.org/wiki/C99).

The 6502 was widely used in early home computers and game consoles like the [Commodore 64](https://en.wikipedia.org/wiki/Commodore_64) and the [Nintendo Entertaiment System](https://en.wikipedia.org/wiki/Nintendo_Entertainment_System).

I chose C99 since I want as little language abstractions as possible, and also, there's something about emulating a vintage system using vintage C.

## Features

- CPU/memory model, and basic operations.
- Implementation of [opcodes](http://www.6502.org/tutorials/6502opcodes.html) (still in progress).
- Testing of capabilities using [CUnit](https://cunit.sourceforge.net).

## Getting started

### Compile

The main function in this project just runs the tests:

```shell
make test
```

### Debug

Additionally, you can choose to debug the test execution, for that you can make a debug build and then debug using GDB or LLDB.

```shell
make debug-build
```
