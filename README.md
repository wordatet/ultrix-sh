# ultrix-sh: Ultrix 4.2 Bourne Shell Port

`ultrix-sh` is a modern 64-bit Linux port of the historical Bourne shell from **ULTRIX V4.2 (Rev. 96)**. 

This project aims to preserve the technical heritage of the Ultrix operating system by making its iconic command interpreter available on contemporary systems while maintaining strict adherence to the original K&R C style and behavior.

## Key Features

- **64-bit Stability**: Refactored for modern LP64 architectures (polymorphic `execexp` and `intptr_t` conversions).
- **Modern Memory Management**: Replaced legacy `sbrk` with a robust `malloc`-based allocator featuring a magic-header "safe free" mechanism.
- **Interactive Ready**: Restored interactive prompt (`$ `) and terminal compatibility via `gtty`/`stty` shims.
- **Historical Accuracy**: Includes the original `sh.1` manual page and maintains historical syntax (e.g., `VAR=VAL; export VAR`).

## Technical Overview

The port addresses several critical challenges encountered when moving 1990-era Unix source to modern Linux:
- **Memory Layout**: Transitioned from a contiguous BSS-to-Heap assumption to a standard heap model.
- **Pointer Punning**: Updated `Rcheat`/`Lcheat` macros to be 64-bit safe.
- **Strict Prototyping**: Added comprehensive prototypes to `defs.h` to satisfy modern GCC requirements while keeping the K&R implementation intact.

## Build Instructions

To build `ultrix-sh` on a modern Linux system:

```bash
make
```

The resulting binary `sh` can be run locally:
```bash
./sh
```

## Running Tests

A basic stress test is provided to verify variable expansion, pipes, and subshell behavior:
```bash
# Apply executable bit if needed
chmod +x stress.sh
./stress.sh
```

## License & Disclaimer

This project is licensed under the **BSD-3-Clause License**. See the [LICENSE](LICENSE) file for the full text and important historical preservation disclaimers.

---
*Preserved and ported by Mario (@wordatet) for research and education.*
