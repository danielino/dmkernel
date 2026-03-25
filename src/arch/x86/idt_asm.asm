; Declare global symbols so the linker can see them
global idt_load
global isr0
global irq0
global irq1
global irq2
global irq3
global irq4
global irq5
global irq6
global irq7
global irq8
global irq9
global irq10
global irq11
global irq12
global irq13
global irq14
global irq15


; This is implemented in C
extern isr0_handler
extern irq_dispatch

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

%macro IRQ 1                                                                                                                                                                                  
irq%1:
    pusha                                                                                                                                                                                     
    push dword %1                                                                                                                                                                           
    call irq_dispatch                                                                                                                                                                         
    add esp, 4      ; remove parameter from stack
    popa                                                                                                                                                                                      
    iret                                                                                                                                                                                    
%endmacro 

IRQ 0                                                                                                                                                                                         
IRQ 1                                                                                                                                                                                         
IRQ 2                                                                                                                                                                                       
IRQ 3                                                                                                                                                                                         
IRQ 4                                                                                                                                                                                         
IRQ 5
IRQ 6                                                                                                                                                                                         
IRQ 7                                                                                                                                                                                       
IRQ 8                                                                
IRQ 9
IRQ 10
IRQ 11
IRQ 12
IRQ 13
IRQ 14
IRQ 15      