#ifndef VMM_H
#define VMM_H

#include <stdint.h>

#define PAGE_PRESENT 0x1
#define PAGE_WRITABLE 0x2
#define PAGE_USER 0x4

typedef uint32_t page_entry_t;
typedef page_entry_t page_table_t[1024];
typedef page_entry_t page_directory_t[1024];

void vmm_init();
void vmm_map_page(page_directory_t* dir, uint32_t virt, uint32_t phys, uint32_t flags);

#endif