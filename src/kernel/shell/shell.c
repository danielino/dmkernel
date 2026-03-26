#include "kernel/shell/shell.h"

#include "arch/x86/io.h"
#include "drivers/keyboard.h"
#include "kernel/shell/commands.h"
#include "kernel/terminal.h"
#include "kernel/timer.h"
#include "kernel/utils/printk.h"
#include "kernel/utils/string.h"
#include "mm/heap.h"
#include "mm/pmm.h"

#define CMD_BUFFER_SIZE 256
#define NUM_COMMANDS (sizeof(commands) / sizeof(commands[0]))
typedef struct {
	const char* name;
	const char* description;
	void (*handler)(const char* args);
} shell_command_t;

void cmd_echo(const char* args)
{
	printk("%s\n", args);
}

void cmd_uptime(const char* args)
{
	printk("Seconds: %d\n", timer_get_ticks() / TIMER_HZ);
}

void sleep(uint32_t seconds)
{
	uint32_t start = timer_get_ticks();
	while (timer_get_ticks() - start < TIMER_HZ * seconds)
		;
}

void cmd_reboot(const char* args)
{
	printk("Rebooting system...\n");
	sleep(1);
	printk(KERN_INFO "disabling hardware interrupt..\n");
	// disable hardware interrupt
	sleep(1);

	outb(0x21, 0xFF);
	outb(0xA1, 0xFF);

	// reboot
	outb(0x64, 0xFE);
}

static shell_command_t commands[] = {
	{"help", "show available commands", cmd_help},
	{"clear", "clear the screen", cmd_clear},
	{"meminfo", "show memory info", cmd_meminfo},
	{"memtest", "test memory", cmd_memtest},
	{"echo", "print text", cmd_echo},
	{"uptime", "show uptime ticks", cmd_uptime},
	{"reboot", "reboot system", cmd_reboot},
};

static void cmd_help(const char* args)
{
	for (int i = 0; i < NUM_COMMANDS; i++)
		printk("  %s - %s\n", commands[i].name, commands[i].description);
}

static void shell_execute(const char* cmd)
{
	int i = 0;
	while (cmd[i] != ' ' && cmd[i] != '\0')
		i++;
	const char* args = (cmd[i] == ' ') ? cmd + i + 1 : "";

	for (int j = 0; j < NUM_COMMANDS; j++) {
		if (strncmp(cmd, commands[j].name, i) == 0 && commands[j].name[i] == '\0') {
			commands[j].handler(args);
			return;
		}
	}
	if (strlen(cmd) > 0)
		printk("Unknown command: %s\n", cmd);
}

void shell_run(void)
{
	char cmd[CMD_BUFFER_SIZE];

	printk("dmkernel shell. Type 'help' for commands.\n");

	while (1) {
		printk("> ");
		while (!keyboard_line_ready())
			; // aspetta Enter
		keyboard_readline(cmd, CMD_BUFFER_SIZE);
		shell_execute(cmd);
	}
}