<!--
==============================================================================
File:        README.md
Description: Official technical documentation for Crusan & Co. AxisOS_Flasher (axflash) — Pure C11/POSIX SLIP & ROM Bootloader Flasher for ESP-Series Micro-Nodes.
Author:      Marc F. Crusan (MFC) / Crusan & Co. w/ PP & AGY (AXEL)
Date:        2026110901
Version:     v1.2.0-Silicon
==============================================================================
-->
<!-- 2026110901 (MFC/PP/AGY) Created canonical 5-Field SOP README.md with Crusan & Co. IP & Trademark notice. -->

# Crusan & Co. — AxisOS_Flasher (`axflash`)
### **Deterministic Pure C / POSIX SLIP & ROM Bootloader Flasher for AxisOS™ Micro-Nodes (`Cub` / ESP32 / ESP32-S3)**

[![Pillars: HONEST • AUDITABLE • OWNED](https://img.shields.io/badge/AxisOS%E2%84%A2-HONEST%20%E2%80%A2%20AUDITABLE%20%E2%80%A2%20OWNED-brightgreen.svg)](https://axisos.pro)
[![Language: C11 / POSIX](https://img.shields.io/badge/Language-Pure%20C11%20%2F%20POSIX-blue.svg)](Makefile)
[![Company: Crusan & Co.](https://img.shields.io/badge/Company-Crusan%20%26%20Co.-gold.svg)](https://axisos.pro)

**Parent Enterprise:** Crusan & Co.  
**Flagship Platform:** **AxisOS™** (*U.S. Trademark Application Serial No. `99878469`*)  
**Architect:** Marc F. Crusan  
**Website:** [https://axisos.pro](https://axisos.pro)  

> **Intellectual Property & Proprietary Core Exclusion Notice:** This repository provides the host-side **AxisOS_Flasher (`axflash`)** serial ROM bootloader and RFC 1055 SLIP transport utility engineered by **Crusan & Co.** The underlying **AxisOS™ Native Bare-Metal Kernel** (`axisos-kernel.img`), the **freestanding C99 direct kernel syscall userland** (`svc #0`), the **Sovereign Seven (`Sov-7`)** autonomous embedded daemon suite, `RAMGAK` deterministic memory bracket tooling, and commercial AxisOS™ hardware appliance distributions remain the exclusive, proprietary intellectual property of **Crusan & Co.** (Marc F. Crusan). All Rights Reserved.

---

## 1. Overview & First-Principles Philosophy

Conventional microcontroller flashing toolchains force operators to install gigabytes of brittle Python virtual environments, opaque pip dependency trees, and external cloud telemetry wrappers just to push a binary image across a UART wire.

**`AxisOS_Flasher` (`axflash`)** is built on the Three Pillars of **AxisOS™**:
- **HONEST**: Direct POSIX `termios` serial control (`DTR`/`RTS` hardware reset sequencing) and explicit ROM bootloader status verification on every frame.
- **AUDITABLE**: Under 1,000 lines of strict, zero-warning C (`-Wall -Wextra -Werror -std=c11`) with zero external libraries—not even `zlib` or Python.
- **OWNED**: Compiles in under one second on any POSIX workstation or directly inside an air-gapped **AxisOS™** node to provision downstream ESP-series micro-nodes (`Cub`).

---

## 2. Architecture & Module Map

| Module | Source Files | Responsibility |
| :--- | :--- | :--- |
| **CLI & Orchestrator** | `main.c` | Command-line parsing (`--port`, `--baud`, `--dry-run`, `--test-reset`), image verification, and flash workflow orchestration |
| **POSIX UART & Auto-Reset** | `serial.c`, `serial.h` | Raw `termios` UART configuration (`115200`–`921600` baud) and deterministic `DTR`/`RTS` hardware bootloader entry sequence |
| **RFC 1055 SLIP Framing** | `slip.c`, `slip.h` | Byte-exact SLIP packet encoding/decoding (`0xC0` frame delimiter, `0xDB` escape handling) over raw serial file descriptors |
| **ROM Bootloader Packet Layer** | `bootrom_pkt.c`, `bootrom_pkt.h`, `bootrom_cmd.h` | Little-endian command/response packet serialization and XOR checksum calculation (`0xEF` seed) |
| **ROM Handshake & Sync** | `bootrom.c`, `bootrom.h` | `ESP_SYNC` (`0x08`) synchronization burst, `ESP_SPI_ATTACH` (`0x0D`), and dynamic baud-rate switching (`ESP_CHANGE_BAUDRATE`) |
| **Flash Engine** | `flash.c`, `flash.h` | Chunked flash erase/write transaction loop (`ESP_FLASH_BEGIN`, `ESP_FLASH_DATA`, `ESP_FLASH_END`) with progress telemetry |
| **Firmware Image Loader** | `image.c`, `image.h` | Binary image validation (`0xE9` magic byte inspection) and memory-safe buffer loading |

---

## 3. Building `AxisOS_Flasher`

No external dependencies are required beyond a standard C compiler (`gcc` or `clang`) and GNU `make`:

```bash
make clean && make
```

Or use the included build script:

```bash
./build-flasher.sh
```

This produces the standalone `./flasher` binary with `-Wall -Wextra -Werror -std=c11`.

---

## 4. Usage

### Flash a Binary Image at `0x10000`
```bash
./flasher --port /dev/ttyUSB0 --baud 115200 0x10000 firmware.bin
```

### Dry-Run Validation (Verify Image & Layout Without Touching Hardware)
```bash
./flasher --dry-run 0x10000 firmware.bin
```

### Test Hardware Auto-Reset (`DTR`/`RTS` Bootloader Entry Sequence)
```bash
./flasher --port /dev/ttyUSB0 --test-reset
```

---

**Copyright © 2026 Crusan & Co. (Marc F. Crusan). All Rights Reserved.**  
**AxisOS™** is a trademark of Crusan & Co. (*U.S. Trademark Application Serial No. `99878469`*).
