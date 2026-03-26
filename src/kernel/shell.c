#include "kernel/shell.h"
#include "kernel/terminal.h"
#include "kernel/timer.h"
#include "kernel/utils/printk.h"
#include "kernel/utils/string.h"
#include "drivers/keyboard.h"
#include "mm/pmm.h"
#include "mm/heap.h"

#define CMD_BUFFER_SIZE 256
#define NUM_COMMANDS (sizeof(commands) / sizeof(commands[0]))
typedef struct {
    const char *name;
    const char *description;
    void (*handler)(const char *args);
} shell_command_t;

static void cmd_help(const char *args);


void cmd_clear(const char *args){
    terminal_clear();
}

void cmd_meminfo(const char *args){
    int mem_free = pmm_free_frames() * 4;
    int mem_total = pmm_total_frames() * 4;
    int mem_used = mem_total - mem_free;
    printk("Total memory: %d KB - Free memory: %d KB - Used memory: %d KB\n", mem_total, mem_free, mem_used);
}

void cmd_memtest(const char *args){                                                                                                                                                           
    int before = pmm_free_frames() * 4;                                                                                                                                                       
    void *ptr = kmalloc(50 * 1024 * 1024);                                                                                                                                             
    int after = pmm_free_frames() * 4;                                                                                                                                                      
    printk("Allocated at: 0x%x\n", (uint32_t)ptr);                                                                                                                                            
    printk("Before: %d KB free - After: %d KB free\n", before, after);                                                                                                                        
    kfree(ptr);                                                                                                                                                                               
    int after_free = pmm_free_frames() * 4;                                                                                                                                                   
    printk("After free: %d KB free (bump allocator: no change expected)\n", after_free);                                                                                                      
}

void cmd_echo(const char *args){
    printk("%s\n", args);
}

 void cmd_uptime(const char *args){
    printk("Seconds: %d\n", timer_get_ticks() / TIMER_HZ);
}


static shell_command_t commands[] = {
    { "help",    "show available commands", cmd_help    },
    { "clear",   "clear the screen",        cmd_clear   },
    { "meminfo", "show memory info",        cmd_meminfo },
    { "memtest", "test memory",             cmd_memtest },
    { "echo",    "print text",              cmd_echo    },
    { "uptime",  "show uptime ticks",       cmd_uptime  },
};

static void cmd_help(const char *args) {
    for (int i = 0; i < NUM_COMMANDS; i++)
        printk("  %s - %s\n", commands[i].name, commands[i].description);
}



static void shell_execute(const char *cmd) {
      int i = 0;
      while (cmd[i] != ' ' && cmd[i] != '\0') i++;
      const char *args = (cmd[i] == ' ') ? cmd + i + 1 : "";

      for (int j = 0; j < NUM_COMMANDS; j++) {
          if (strncmp(cmd, commands[j].name, i) == 0 &&
              commands[j].name[i] == '\0') {
              commands[j].handler(args);
              return;
          }
      }
      if (strlen(cmd) > 0)
          printk("Unknown command: %s\n", cmd);
  }

void shell_run(void) {
    char cmd[CMD_BUFFER_SIZE];

    printk("dmkernel shell. Type 'help' for commands.\n");

    while (1) {
        printk("> ");
        while (!keyboard_line_ready());  // aspetta Enter
        keyboard_readline(cmd, CMD_BUFFER_SIZE);
        shell_execute(cmd);
    }
}