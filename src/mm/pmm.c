#include "mm/pmm.h"

#include "kernel/utils/printk.h"
#include "multiboot.h"

#define MAX_FRAMES 32768

extern uint32_t kernel_end;
static uint32_t* bitmap = &kernel_end;
// static uint32_t bitmap[MAX_FRAMES / 32];
static uint32_t total_frames = 0;
static uint32_t free_frames = 0;

static void bitmap_set(uint32_t frame)
{
	bitmap[frame / 32] |= (1 << (frame % 32));
}

static void bitmap_clear(uint32_t frame)
{
	bitmap[frame / 32] &= ~(1 << (frame % 32));
}

static int bitmap_test(uint32_t frame)
{
	return bitmap[frame / 32] & (1 << (frame % 32));
}

void pmm_init(multiboot_info_t* mbi)
{

	total_frames = mbi->mem_upper * 1024 / PAGE_SIZE;
	free_frames = 0;

	uint32_t bitmap_size = total_frames / 32;
	for (uint32_t i = 0; i < bitmap_size; i++)
		bitmap[i] = 0xFFFFFFFF;

	// iterate over multiboot memory map
	multiboot_mmap_entry_t* entry = (multiboot_mmap_entry_t*)mbi->mmap_addr;
	multiboot_mmap_entry_t* end = (multiboot_mmap_entry_t*)(mbi->mmap_addr + mbi->mmap_length);

	while (entry < end) {
		if (entry->type == 1) {
			uint32_t frame = entry->addr / PAGE_SIZE;
			uint32_t count = entry->len / PAGE_SIZE;
			for (uint32_t i = 0; i < count; i++) {
				bitmap_clear(frame + i);
				free_frames++;
			}
		}

		entry = (multiboot_mmap_entry_t*)((uint32_t)entry + entry->size + sizeof(uint32_t));
	}

	uint32_t kernel_end_frame = ((uint32_t)&kernel_end + bitmap_size * 4) / PAGE_SIZE + 1;
	for (uint32_t i = 0; i < kernel_end_frame; i++) {
		if (bitmap_test(i))
			continue;
		bitmap_set(i);
		free_frames--;
	}

	printk(KERN_INFO "pMM: %d KB free (%d frames)\n", free_frames * 4, free_frames);
}

void* pmm_alloc_frame(void)
{
	if (!free_frames)
		return 0;

	for (uint32_t i = 0; i < total_frames; i++) {
		if (!bitmap_test(i)) {
			bitmap_set(i);
			free_frames--;
			return (void*)(i * PAGE_SIZE);
		}
	}
	return 0;
}

void pmm_free_frame(void* addr)
{
	uint32_t frame = (uint32_t)addr / PAGE_SIZE;
	if (bitmap_test(frame)) {
		bitmap_clear(frame);
		free_frames++;
	}
}

uint32_t pmm_free_frames(void)
{
	return free_frames;
}

uint32_t pmm_total_frames(void)
{
	return total_frames;
}