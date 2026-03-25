section .multiboot
align 4
    dd 0x1BADB002          ; magic
    dd 0x00000003          ; flags (align + memory info)
    dd -(0x1BADB002 + 0x00000003)

section .text
global _start

_start:
    extern kernel_main
    call kernel_main

.hang:
    cli
    hlt
    jmp .hang