#include <stdint.h> 

#include "drivers/keyboard.h"
#include "kernel/terminal.h"
#include "arch/x86/pic.h"
#include "arch/x86/io.h"


static const char scancode_map[] = {        
    0,   0,   '1', '2', '3', '4', '5', '6',
    '7', '8', '9', '0', '\'','i', 0,   0,   
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i',                                                                                                                                                   
    'o', 'p', 'e', '+', 0,   0,   'a', 's', 
    'd', 'f', 'g', 'h', 'j', 'k', 'l', 'o',                                                                                                                                                   
    'a', '\\',0,   'u', 'z', 'x', 'c', 'v',                                                                                                                                                   
    'b', 'n', 'm', ',', '.', '-', 0,   0,                                                                                                                                                     
    0,   ' '                                                                                                                                                                                  
};                                                                                                                                                                                            
                                                                                                                                                                                            
                                                                                                                                                                                            
static const char scancode_map_shift[] = {                                                                                                                                                    
    0,   0,   '!', '"', '#', '$', '%', '&',                                                                                                                                                 
    '/', '(', ')', '=', '?', '^', 0,   0,                            
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 
    'O', 'P', 'E', '*', 0,   0,   'A', 'S', 
    'D', 'F', 'G', 'H', 'J', 'K', 'L', 'O',                                                                                                                                                   
    'A', '|', 0,   'U', 'Z', 'X', 'C', 'V',                                                                                                                                                   
    'B', 'N', 'M', ';', ':', '_', 0,   0,                                                                                                                                                     
    0,   ' '                                                                                                                                                                                  
}; 

static int shift_pressed = 0;               
                                          
void irq1_handler() {
    uint8_t scancode = inb(0x60);                                                                                                                                                             
                                            
    if (scancode == 0x2A || scancode == 0x36) {                                                                                                                                               
        shift_pressed = 1;                                                                                                                                                                  
        pic_send_eoi(1);                                                                                                                                                                      
        return;                             
    }                                                                                                                                                                                         
    if (scancode == 0xAA || scancode == 0xB6) {                                                                                                                                             
        shift_pressed = 0;                                                                                                                                                                    
        pic_send_eoi(1);                
        return;                                                                                                                                                                               
    }                                                                                                                                                                                         
                                                                    
    if (scancode & 0x80) {                                                                                                                                                                    
        pic_send_eoi(1);                                                                                                                                                                    
        return;                                                      
    }                                                                                                                                                                                         

    if (scancode < sizeof(scancode_map)) {                                                                                                                                                    
        char c = shift_pressed ? scancode_map_shift[scancode] : scancode_map[scancode];                                                                                                     
        if (c != 0) {                                                
            char buf[2] = {c, 0};       
            terminal_write(buf);
        }                                                                                                                                                                                     
    }                                       
                                                                                                                                                                                            
    pic_send_eoi(1);                                                                                                                                                                        
}   