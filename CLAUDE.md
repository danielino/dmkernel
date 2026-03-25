# Project: Minimal x86 Kernel (Educational OS Development)

## Overview

This project is a minimal educational operating system kernel written in C and x86 assembly.

It runs in a virtual machine using QEMU and is booted via GRUB using the Multiboot specification.

The purpose of this project is to understand low-level systems concepts such as:

* boot process
* memory layout
* interrupts (IDT)
* hardware interaction
* OS architecture

This is NOT a userland program. This is bare-metal development.

---

## Environment

* Host OS: macOS
* Emulator: QEMU (qemu-system-x86_64)
* Toolchain:

  * i686-elf-gcc (cross compiler)
  * NASM (for assembly)
  * GRUB (grub-mkrescue)

Compilation is freestanding (no libc).

---

## Build System

Use:

```
make
make run
```

Artifacts are generated inside:

```
build/
```

Do not place build artifacts in the project root.

---

## Architecture

The project is structured as follows:

```
src/        → implementation (C code)
include/    → headers (mirrors src structure)
arch/x86/   → architecture-specific code (IDT, IO, linker)
boot/       → bootloader entry (assembly)
build/      → build output (generated)
```

Key components:

* VGA driver (text mode output)
* Terminal abstraction (higher-level text output)
* IDT (Interrupt Descriptor Table)
* Basic ISR handling (e.g. divide-by-zero)

---

## Important Constraints

* No standard library (no printf, malloc, etc.)
* Must compile with:
  -ffreestanding
  -m32
* No OS services available
* Direct hardware interaction (ports, memory-mapped IO)
* Must be deterministic and low-level

---

## Assembly & C Interop

* Assembly is used for:

  * interrupt stubs
  * low-level CPU instructions (lidt, cli, etc.)

* C is used for:

  * kernel logic
  * drivers
  * higher-level abstractions

* Functions declared in assembly with `extern` must be implemented in C

* Be careful with calling conventions and stack usage

---

## Interrupt Model

* CPU generates interrupts/exceptions
* IDT maps interrupt numbers → handler addresses
* Assembly ISR acts as entry point
* C handler contains logic

Example flow:

```
CPU exception → IDT → isr (asm) → handler (C)
```

---

## Current State

Working features:

* Kernel boots via GRUB
* VGA text output works
* IDT is initialized
* Divide-by-zero exception is handled

---

## Next Steps (Planned)

* PIC (Programmable Interrupt Controller) setup
* Hardware interrupts (IRQ)
* Keyboard driver (IRQ1)
* Timer (IRQ0)
* Improved ISR handling (register saving, iret)
* Paging / memory management (future)

---

## What Claude Should Do

* Help user implementing low-level kernel features incrementally
* Suggest minimal, understandable code
* Explain hardware concepts clearly when needed
* Keep everything compatible with freestanding C
* Avoid high-level abstractions or OS-dependent code

---

## What Claude Must NOT Do

* Do NOT use standard library functions
* Do NOT assume a hosted environment
* Do NOT introduce complex frameworks
* Do NOT hide low-level details behind abstractions
* MANDATORY: DO NOT write code, claude must be used only to guide user and explain code/functionality. you must NEVER touch or write codebase, with a single specific exception for README.md and .gitignore file. also claude can use git to create commit and push to remote repository to speed up developer experience

---

## Notes

This project is educational. Correctness and clarity are more important than performance or completeness.
