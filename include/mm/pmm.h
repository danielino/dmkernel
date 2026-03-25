#ifndef PMM_H
#define PMM_H

#include "multiboot.h"

#define PAGE_SIZE 0x1000 // (4096)


void pmm_init(multiboot_info_t *mbi);
void *pmm_alloc_frame(void); // frame allocation, return physical addr
void pmm_free_frame(void *addr);
uint32_t pmm_free_frames(void);

#endif