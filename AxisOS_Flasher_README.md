<!--
-----------------------------------------------------------------------------
 File:        AxisOS_Flasher_README.md
 Description: ESP32-S class chip 'flasher'
 Author:      Marc F Crusan / PP
 Date:        20260623
 Version:     v0.0.1
-----------------------------------------------------------------------------
-->
# 20260623 (MFC) File Created v0.0.1
# “AxisOS™ micro-enterprise solutions”
# ESP‑Series Flasher
*A lightweight, dependency‑free, C‑based flasher for ESP32‑S‑class chips*  
Part of the emerging **AxisOS Toolchain**

**Status:** Early development (v0.0.1) — S‑series support implemented, multi‑chip roadmap planned.

## 🚀 Overview
The **ESP‑Series Flasher** is a fast, minimal, dependency‑free firmware flashing tool written in C.  
It implements the modern Espressif boot ROM protocol used by:

- ESP32‑S2  
- ESP32‑S3  
- ESP32‑C3  
- ESP32‑C6  
- ESP32‑H2  

The tool is designed for:

- embedded developers  
- automation pipelines  
- manufacturing workflows  
- custom OS toolchains (like AxisOS)  
- CLI‑driven environments  
- low‑overhead flashing on Linux systems  

This project aims to provide a clean, modern alternative to Python‑based tools like `esptool.py`, with a focus on speed, clarity, and embeddability.

## ✨ Features
- Modern ESP32‑S‑series bootloader protocol  
- SLIP‑wrapped packet layer  
- Flash erase, write, and MD5 verification  
- Automatic bootloader entry via DTR/RTS  
- Zero external dependencies  
- Clean, modular C architecture  
- Ready for multi‑chip expansion  
- Designed for integration with PP‑CLI and AxisOS

## 🧩 Supported Chips
- ESP32‑S2  
- ESP32‑S3  
- ESP32‑C3  
- ESP32‑C6  
- ESP32‑H2  

## ⚠️ Limitations
- ESP8266 is not yet supported  
- ESP32 classic (D0WD/WROOM) uses a different protocol and is not yet implemented  
- No stub loader support (planned)  
- No secure‑boot or flash‑encryption handling (planned)

## 📦 Building
Requirements:

- GCC or Clang  
- POSIX‑compatible system (Linux, macOS, BSD)

Build:

```
make
```

Clean:

```
make clean
```

The resulting binary will be:

```
./flasher
```

## 🔧 Usage
Basic flashing:

```
./flasher --port /dev/ttyUSB0 --baud 460800 write firmware.bin 0x0000
```

Erase flash:

```
./flasher --port /dev/ttyUSB0 erase
```

Verify MD5:

```
./flasher --port /dev/ttyUSB0 verify firmware.bin 0x0000
```

Reset device:

```
./flasher --port /dev/ttyUSB0 reset
```

## 🧠 Architecture
The flasher is composed of clean, modular components:

- bootrom.c — bootloader command interface  
- bootrom_pkt.c — SLIP‑wrapped packet encoding  
- flash.c — erase/write/verify operations  
- serial.c — POSIX serial I/O  
- slip.c — SLIP framing  
- image.c — firmware image helpers  
- main.c — CLI entry point  

This structure makes it easy to add support for additional chip families.

## 🧭 Roadmap
Planned features and expansions:

- ESP32 classic support  
- ESP8266 support  
- ESP32‑C series enhancements  
- Chip family auto‑detection  
- Protocol abstraction layer  
- Stub loader support  
- PP‑CLI integration  

## 🤝 Contributing
Contributions are welcome.  
Please see **CONTRIBUTING.md** for guidelines on:

## ⭐ SOP v1.2 Compliance
All modules must include the full SOP header and follow the 10‑point standard:

1. Clean compilation (-Wall -Wextra)
2. Clear memory ownership
3. No silent failures
4. No undocumented globals
5. Deterministic behavior
6. SOP header required
7. Readable, maintainable code
8. No magic numbers
9. No undefined behavior
10. No animals harmed


Full SOP included in `AxisOS_Flasher_SOP_v1.2.md`.


- AxisOS SOP compliance, a living document.
- coding style  
- commit conventions  
- issue reporting  
- pull request workflow  

## 📄 License
Apache 2.0 [Avalible Online]

## 🧩 Part of the AxisOS Ecosystem
This flasher is the first public tool in the AxisOS toolchain, which will include:

- PP‑CLI (Pocket Professor CLI)  
- AxisOS Defense Orchestrator  
- Embedded viewers (hex, packet, log)  
- Multi‑node micro‑enterprise cluster tools  

This project is the foundation for a broader ecosystem of developer‑focused tools.
```

---

