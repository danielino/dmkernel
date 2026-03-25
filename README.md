# my-os

A minimal x86 kernel written in C and assembly, built for educational purposes.

Runs inside QEMU and boots via GRUB using the Multiboot specification.

---

## Goals

Understand low-level systems concepts from scratch:

- Boot process and GRUB Multiboot
- VGA text mode output
- Interrupt handling (IDT, ISR)
- Hardware interrupts via PIC (IRQ)
- Keyboard input with Italian layout
- Timer via PIT (IRQ0)

---

## Requirements

| Tool | Purpose |
|------|---------|
| `i686-elf-gcc` | Cross-compiler (freestanding C) |
| `nasm` | Assembler |
| `grub-mkrescue` | Creates bootable ISO |
| `qemu-system-x86_64` | Emulator |

On macOS, install via Homebrew:

```bash
brew install nasm qemu
brew install i686-elf-gcc i686-elf-binutils
brew install i686-elf-grub
```

---

## Build & Run

```bash
make        # build kernel.iso
make run    # launch in QEMU
make clean  # remove build artifacts
```

---

## Project Structure

```
boot/           → GRUB Multiboot entry point (assembly)
arch/x86/       → Linker script
src/
  arch/x86/     → IDT, PIC, I/O ports, ISR stubs (assembly)
  drivers/       → VGA driver, keyboard driver
  kernel/        → Terminal, timer, kernel main
include/        → Headers (mirrors src/ structure)
build/          → Generated artifacts (not committed)
```

---

## Features

- Boots via GRUB (Multiboot spec)
- VGA text mode output with cursor support
- IDT initialized with exception and IRQ handlers
- PIC (8259) remapped to avoid conflicts with CPU exceptions
- Divide-by-zero exception handler (INT 0)
- Timer interrupt (IRQ0) with tick counter
- Keyboard driver (IRQ1) with Italian PS/2 scancode layout
  - Shift support (uppercase + symbols)
  - Backspace with visual feedback
  - Input buffer (255 chars)

---

## Architecture

```
CPU exception / hardware IRQ
        ↓
      IDT entry
        ↓
   ISR stub (assembly)   ← saves registers, calls C handler
        ↓
   C handler             ← kernel logic
        ↓
      iret               ← returns to interrupted code
```

The PIC remaps IRQ0–IRQ15 to interrupts 0x20–0x2F to avoid collisions with CPU exceptions (0x00–0x1F).

---

## Constraints

- No standard library (`-ffreestanding`, no libc)
- Compiled for 32-bit x86 (`-m32`)
- No OS services — direct hardware interaction only
- No dynamic memory allocation (no `malloc`)

---

## Next Steps

- [ ] Shell with command parsing
- [ ] `strcmp`, `memcpy`, `strlen` utilities
- [ ] Paging and virtual memory
- [ ] Memory allocator
