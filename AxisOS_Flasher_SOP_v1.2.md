# **AxisOS Flasher — Standard Operating Procedure (SOP v1.2)**
### *Effective Date: 2026‑06‑04*
### *Applies to: All Flasher modules, plugins, utilities, and future expansions*
### *Donations are always welcome
### **<a href='https://www.paypal.com/donate/?hosted_button_id=9VSLMAJVS74CA'>Donate via PayPal</a>**
---

## **1. Purpose**
The AxisOS Flasher SOP defines the engineering, formatting, architectural, and operational standards required for all modules within the AxisOS family.
This ensures:

- deterministic behavior
- maintainable code
- consistent UI/UX
- plugin ABI stability
- zero drift across modules
- reproducibility across builds

This SOP is mandatory for all Flasher components.

---

## **2. Scope**
This SOP applies to:
- `*.c`, `*.h`, `*.txt`, `*.md`, `*.db`, `*.sql`, `*.man`, `*.sh` and **Others as necessary**
- all plugins under `plugins/`
- all future Flasher modules

It does **not** apply to AxisOS kernel modules, PP‑CLI, or external utilities unless explicitly adopted.

---

## **3. SOP Header Requirements**
Every source file **must** begin with the following header block, with only:

- filename
- purpose
- creation date

modified:

```
/*==============================================================================
 *  FILE:        <filename>
 *  PROJECT:     AxisOS — Flasher
 *  PURPOSE:     <short module purpose — 1 line>
 *
 *  AUTHOR:      Marc & PP (Pocket Professor)
 *  CREATED:     <YYYY-MM-DD>
 *  VERSION:     1.2
 *
 *  STANDARD OPERATING PROCEDURE — AxisOS Code Compliance
 *  ---------------------------------------------------------------------------
 *  1. All modules must compile cleanly under GCC and Clang with -Wall -Wextra.
 *  2. No dynamic memory without ownership clarity; all allocations paired with
 *     deterministic frees or owned by GTK lifecycle.
 *  3. No silent failures — all errors must be logged or surfaced to caller.
 *  4. No global state except where explicitly documented and justified.
 *  5. All UI components must follow dual-pane viewer architecture:
 *         LEFT  = numeric view (Hex/Oct/Bin)
 *         RIGHT = ASCII view
 *     with synchronized vertical scrolling and consistent monospace rendering.
 *  6. All modules must include SOP headers, consistent naming, and no drift.
 *  7. All code must be readable, maintainable, and free of legacy artifacts.
 *  8. All viewer output must be deterministic and reproducible.
 *  9. All plugins must follow the AxisOS Plugin ABI (v1.0).
 * 10. No animals were harmed in the making of this software.
 *
 *  NOTES:
 *      - This file is part of the AxisOS Flasher distribution.
 *      - Any modification must preserve SOP compliance.
 *      - All viewer modules must remain hot‑reload safe.
 *============================================================================*/
```

---

## **4. Coding Standards**

### **4.1 Compiler Flags**
All modules must compile cleanly under:

- `gcc`
- `clang`

With:

```
-Wall -Wextra -Werror=implicit-function-declaration
```

Warnings are treated as defects.

---

### **4.2 Memory Ownership**
- No heap allocations without explicit ownership.
- GTK‑owned objects must not be manually freed.
- All `g_string_new()` allocations must be paired with `g_string_free()`.

---

### **4.3 Logging**
- No silent failures.
- Use `g_warning()`, `g_error()`, or `g_message()` appropriately.
- Error messages must include filename or context.

---

### **4.4 Global State**
Global variables are allowed **only** for:

- text buffers
- text views
- shared adjustments
- plugin registry pointers

All globals must be documented in the header block.

---

## **5. Viewer Architecture Requirements**

### **5.1 Dual‑Pane Layout**
All Flasher windows must follow the canonical layout:

- **Left pane:** Numeric view (Hex, Octal, Binary)
- **Right pane:** ASCII representation
- **Shared vertical scroll adjustment**
- **Monospace fonts only**
- **Consistent margins**

### **5.2 Rendering Rules**
- 16 bytes per line
- Address column padded to 8 hex digits
- ASCII pane must always align with numeric pane
- Non‑printable characters rendered as `.`

### **5.3 Pane Behavior**
- Left pane expands
- Right pane maintains minimum width
- Both panes must remain visible at all times
- No horizontal scrollbars unless explicitly required

---

## **6. Plugin ABI Requirements**

### **6.1 ABI Structure**
Each plugin must export:

```c
AxisPluginInfo *plugin_info(void);
```

Where:

```c
typedef struct {
    const char *name;
    const char *description;
    void (*run)(GtkWidget *parent);
} AxisPluginInfo;
```

### **6.2 Plugin Directory**
Plugins must reside in:

```
./plugins/
```

### **6.3 Safety Rules**
- Plugins must not crash the viewer
- Plugins must not modify global state
- Plugins must not block the GTK main loop

---

## **7. UI/UX Standards**

### **7.1 Fonts**
All text views must use:

- monospace
- consistent margins
- consistent padding

### **7.2 Menus**
Required menus:

- File
- Plugins
- Help

### **7.3 Toolbar**
Required buttons:

- Hex
- Oct
- Bin
- About

---

## **8. Build System Standards**

### **8.1 Makefile Requirements**
The Makefile must:

- build to `bin/axis-view-gui`
- support `clean`
- use `pkg-config` for GTK
- place all objects in `src/`

### **8.2 Directory Structure**
```
viewer_files/
    include/
    src/
    plugins/
    bin/
```

---

## **9. Documentation Requirements**
All modules must:

- include SOP header
- include purpose statement
- include notes on ownership
- include ABI notes if applicable

---

## **10. Versioning**
Flasher uses semantic versioning:

- **1.0** — prototype
- **1.1** — drift era
- **1.2** — SOP standardization (this document)
- **1.3+** — future expansions

---

## **11. Compliance**
Any module failing SOP v1.2:

- must be corrected before merge
- must not be shipped
- must not be included in release builds

---

## **12. Humor Clause**
As required by AxisOS tradition:

> **No animals were harmed in the making of this software.
> PP may have been mildly inconvenienced.**
