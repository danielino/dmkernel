; Declare global symbols so the linker can see them
global idt_load
global isr0
global irq0
global irq1

; This is implemented in C
extern isr0_handler
extern irq0_handler
extern irq1_handler

; -------------------------------
; Load IDT into CPU
; -------------------------------
; Argument:
;   [esp + 4] = pointer to idt_ptr
;
; The lidt instruction tells the CPU:
; "Here is where the IDT is located"
;
idt_load:
    mov eax, [esp + 4]   ; Load pointer to idt_ptr into eax
    lidt [eax]           ; Load IDT register (IDTR)
    ret

; -------------------------------
; Interrupt Service Routine 0
; (Divide by zero)
; -------------------------------
isr0:
    cli                  ; Disable interrupts (safety)
    pusha
    ; Call C handler
    call isr0_handler
    popa
    ; Halt CPU (we don't recover yet)
    iret

irq0:
    pusha
    call irq0_handler
    popa
    iret

irq1:
    pusha
    call irq1_handler
    popa
    iret