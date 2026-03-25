#include "mm/pmm.h"
#include "multiboot.h"
#include "kernel/utils/printk.h"

#define MAX_FRAMES 32768

static uint32_t bitmap[MAX_FRAMES / 32];
static uint32_t total_frames = 0;
static uint32_t free_frames = 0;

static void bitmap_set(uint32_t frame) {
    bitmap[frame / 32] |= (1 << (frame % 32));
}

static void bitmap_clear(uint32_t frame) {
    bitmap[frame / 32] &= ~(1 << (frame % 32));
}

static int bitmap_test(uint32_t frame) {
    return bitmap[frame / 32] & (1 << (frame % 32));
}

void pmm_init(multiboot_info_t *mbi){
    for(int i = 0; i < MAX_FRAMES / 32; i++)
        bitmap[i] = 0xFFFFFFFF;

    total_frames = mbi->mem_upper * 1024 / PAGE_SIZE;
    free_frames = 0;

    // iterate over multiboot memory map
    multiboot_mmap_entry_t *entry = (multiboot_mmap_entry_t *)mbi->mmap_addr;
    multiboot_mmap_entry_t *end   = (multiboot_mmap_entry_t *)(mbi->mmap_addr + mbi->mmap_length);

    while(entry < end){
        if(entry->type == 1){
            uint32_t frame = entry->addr / PAGE_SIZE;
            uint32_t count = entry->len / PAGE_SIZE;
            for(uint32_t i = 0; i < count; i++){
                bitmap_clear(frame + i);
                free_frames++;
            }
        }

        entry = (multiboot_mmap_entry_t *)((uint32_t)entry + entry->size + sizeof(uint32_t));
    }

    printk(KERN_INFO "pMM: %d KB free (%d frames)\n", free_frames * 4, free_frames);
}

void *pmm_alloc_frame(void){
    if(free_frames) return 0;

    for(uint32_t i = 0; i < MAX_FRAMES ; i++){
        if(!bitmap_test(i)){
            bitmap_set(i);
            free_frames--;
            return (void *)(i * PAGE_SIZE);
        }
    }
    return 0;
}

void pmm_free_frame(void *addr){
    uint32_t frame = (uint32_t)addr / PAGE_SIZE;
    if(bitmap_test(frame)){
        bitmap_clear(frame);
        free_frames++;
    }
}

uint32_t pmm_free_frames(void){
    return free_frames;
}