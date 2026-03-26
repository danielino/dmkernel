  #include "kernel/shell.h"
  #include "kernel/terminal.h"
  #include "kernel/utils/printk.h"
  #include "kernel/utils/string.h"
  #include "drivers/keyboard.h"

  #define CMD_BUFFER_SIZE 256

  static void shell_execute(const char *cmd) {
      if (strcmp(cmd, "help") == 0) {
          printk("Commands: help, clear\n");
      } else if (strcmp(cmd, "clear") == 0) {
          terminal_clear();
      } else if (strlen(cmd) > 0) {
          printk("Unknown command: %s\n", cmd);
      }
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