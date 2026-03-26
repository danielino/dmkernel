#include <stdint.h> 

#include "drivers/keyboard.h"
#include "kernel/terminal.h"
#include "arch/x86/pic.h"
#include "arch/x86/io.h"

#define KEYBOARD_BUFF_SIZE 0xFF
static char buffer[KEYBOARD_BUFF_SIZE];
static int buf_len = 0;

// 1 when press enter
static int line_ready = 0;

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
                                          
void keyboard_irq_handler() {
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
                            
    // key release events
    if (scancode & 0x80) {                                                                                                                                                                    
        pic_send_eoi(1);                                                                                                                                                                    
        return;                                                      
    }           

    // backspace
    if(scancode == 0x0E){
        if (buf_len > 0){
            buf_len--;
            terminal_write("\b");
        }
        pic_send_eoi(1);
        return;
    }

    // enter
    if(scancode == 0x1C){
        line_ready = 1;
        terminal_write("\n"); 
        pic_send_eoi(1);
        return;
    }

    char c = 0;
    if (scancode < sizeof(scancode_map)) {                                                                                                                                                    
        c = shift_pressed ? scancode_map_shift[scancode] : scancode_map[scancode];                                                                                                     
        if (c != 0) {                                                
            buffer[buf_len++] = c;
            buffer[buf_len] = '\0';
            char buf[2] = {c, 0};
            terminal_write(buf); 
        }                                                                                                                                                                                     
    }
                                      
                                                                                                                                                                                            
    pic_send_eoi(1);                                                                                                                                                                        
}   

int keyboard_line_ready(void){
    return line_ready;
}

void keyboard_readline(char *out, int max){
    int len = buf_len < max - 1 ? buf_len : max - 1;
    for(int i = 0; i < len; i++)
        out[i] = buffer[i];

    out[len] = '\0';
    buf_len = 0;
    line_ready = 0;
}