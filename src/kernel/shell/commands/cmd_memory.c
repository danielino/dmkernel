
#include <stdint.h>
#include "mm/pmm.h"
#include "mm/heap.h"
#include "kernel/utils/printk.h"

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