#ifndef MULTIBOOT_H
#define MULTIBOOT_H

#include <stdint.h>

#define MULTIBOOT_MAGIC 0x2BADB002

typedef struct {
	uint32_t flags;
	uint32_t mem_lower; // KB di memoria bassa (sotto 1MB)
	uint32_t mem_upper; // KB di memoria alta (sopra 1MB)
	uint32_t boot_device;
	uint32_t cmdline;
	uint32_t mods_count;
	uint32_t mods_addr;
	uint32_t syms[4];
	uint32_t mmap_length; // dimensione della memory map
	uint32_t mmap_addr;	  // indirizzo della memory map
} __attribute__((packed)) multiboot_info_t;

typedef struct {
	uint32_t size;
	uint64_t addr; // indirizzo fisico della regione
	uint64_t len;  // dimensione in byte
	uint32_t type; // 1 = disponibile, altri = riservato
} __attribute__((packed)) multiboot_mmap_entry_t;

#endif