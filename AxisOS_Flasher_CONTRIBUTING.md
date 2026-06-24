Here it is, Marc — **the real, correct, flasher‑specific CONTRIBUTING.md**, written clean, disciplined, SOP‑aligned, and with *zero* Viewer Suite bleed‑through.  
No GTK.  
No plugins.  
No dual‑pane UI.  
No `.so` files.  
No refresh gremlins.

Just the **AxisOS Flasher**, exactly as it is.

You can drop this straight into:

```
AxisOS_Flasher_CONTRIBUTING.md
```

---

# **AxisOS Flasher — CONTRIBUTING.md (Correct, Clean, SOP‑Aligned)**

```
/* -----------------------------------------------------------------------------
   File:        AxisOS_Flasher_CONTRIBUTING.md
   Project:     AxisOS Flasher
   Description: Contribution guidelines for developers and testers working on the
                AxisOS ESP‑Series Flasher
   Author:      Marc F Crusan / PP
   Version:     v1.0
   SOP:         Fully compliant with SOP v1.2
   ----------------------------------------------------------------------------- */
```

# Contributing to the AxisOS Flasher  
Thank you for your interest in contributing to the **AxisOS Flasher** — a clean, deterministic, SOP‑driven firmware flashing tool for ESP‑series devices.  
Contributions are welcome as long as they maintain the clarity, discipline, and zero‑drift philosophy of the AxisOS ecosystem.

Awards/Rewards:  
- 🍪 Standard job well done  
- 🏆 Milestone achievement  
- 💃🕺 The Happy Dance  
- ⭐ Gold Star (almost a cookie)

---

## ⭐ How to Contribute  
You can contribute by:

- **Reporting issues**  
- **Submitting pull requests**  
- **Improving documentation**  
- **Adding SOP‑compliant modules**  
- **Extending bootrom or packet support**  

All contributions must follow SOP v1.2.

---

## ⭐ SOP v1.2 Requirements  
Every source file **must** include the full SOP header:

```
/* -----------------------------------------------------------------------------
   File:        <filename>
   Description: <what this module does>
   Author:      <your name or initials>
   Date:        <YYYYMMDD>
   Version:     <vX.Y>
   ----------------------------------------------------------------------------- */
```

And must comply with the 10‑point SOP standard:

1. Clean compilation (`-Wall -Wextra`)  
2. Clear memory ownership  
3. No silent failures  
4. No undocumented globals  
5. Deterministic behavior  
6. SOP header required  
7. Readable, maintainable code  
8. No magic numbers  
9. No undefined behavior  
10. No animals harmed  

Full SOP is in `AxisOS_Flasher_SOP_v1.2.md`.

---

## ⭐ Coding Style  
### Indentation  
- 4 spaces  
- No tabs  
- No trailing whitespace  

### Naming  
- Functions: `snake_case`  
- Types: `CamelCase`  
- Constants: `UPPER_SNAKE_CASE`  
- Globals: **avoid unless absolutely required**  

### Error Handling  
- No silent failures  
- All errors must be surfaced  
- Serial/bootrom errors must return actionable codes  
- Flashing operations must fail fast and loudly  

### Memory  
- Ownership must be explicit  
- Free what you allocate  
- No leaks, no orphans, no mysteries  

---

## ⭐ Pull Request Process  
1. Fork the repository  
2. Create a feature branch  
3. Ensure your code builds cleanly  
4. Ensure SOP v1.2 compliance  
5. Update documentation if needed  
6. Submit a pull request with a clear description  

Pull requests that violate SOP v1.2 will be corrected or rejected depending on caffeine levels.

---

## ⭐ Extending the Flasher  
The flasher is modular. You may extend:

- **bootrom commands**  
- **packet types**  
- **SLIP encoding/decoding**  
- **serial transport**  
- **image handling**  

### Bootrom Extensions  
Add new commands in:

- `bootrom_cmd.h`  
- `bootrom.c`  

Ensure:

- Correct opcode  
- Correct argument structure  
- Correct response parsing  

### Packet Layer Extensions  
Modify:

- `bootrom_pkt.c`  
- `bootrom_pkt.h`  

Ensure:

- Deterministic framing  
- Correct checksum behavior  
- No malformed packets  

### Serial Layer  
Modify:

- `serial.c`  
- `serial.h`  

Ensure:

- Non‑blocking reads  
- Clear error propagation  
- No partial‑read ambiguity  

---

## ⭐ Testing  
Before submitting:

- Build with `make`  
- Run the flasher against a test ESP device  
- Verify:  
  - erase  
  - begin  
  - write blocks  
  - end  
- Confirm MD5 verification (if enabled)  
- Confirm no serial timeouts  
- Confirm no packet framing errors  

Automated tests will be added in v1.3.

---

## ⭐ Filing Issues  
When reporting a bug, include:

- Steps to reproduce  
- Expected behavior  
- Actual behavior  
- Device model (ESP32, ESP32‑S3, etc.)  
- Flash size  
- Baud rate  
- Whether cookies were involved  

Use the **Issue Tracker**.

---

## ⭐ Branching Model  
- `main` — stable, tagged releases  
- `dev` — active development  
- `feature/*` — new features  
- `bugfix/*` — fixes  
- `transport/*` — serial/SLIP changes  
- `bootrom/*` — ROM command extensions  

No commits directly to `main`.

---

## ⭐ Contributor Conduct  
We maintain:

- respect  
- clarity  
- professionalism  
- humor  
- zero drift  

Cookies are encouraged.

---

## ⭐ Final Notes  
The AxisOS Flasher is part of the broader AxisOS lineage — PP‑CLI, AxisOS Defense Orchestrator, and the micro‑enterprise toolchain.  
Your contributions help strengthen the foundation of the ecosystem.

Thank you for helping build something crafted, disciplined, and uniquely AxisOS.

---

If you want, I can now generate:

- **AxisOS_Flasher_README.md**  
- **AxisOS_Flasher_Structure.md**  
- **AxisOS_Flasher_SOP_v1.2.md**  

Just tell me which one you want next.
