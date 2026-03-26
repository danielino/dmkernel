#include "mm/heap.h"

#include "kernel/utils/printk.h"
#include "mm/pmm.h"

extern uint32_t kernel_end;

static uint32_t heap_start = 0;
static uint32_t heap_current = 0;
static uint32_t heap_max = 0;

void heap_init(void)
{
	heap_start = (uint32_t)&kernel_end + (pmm_free_frames() / 8);
	heap_current = heap_start;
	heap_max = heap_start + (16 * 1024 * 1024); // 16MB heap size

	printk(KERN_INFO "Heap: start=%x max=%x\n", heap_start, heap_max);
}

void* kmalloc(uint32_t size)
{
	// align to 4 bytes
	size = (size + 3) & ~3;

	if (heap_current + size > heap_max) {
		printk(KERN_ERR "Heap: out of memory\n");
		return 0;
	}

	void* ptr = (void*)heap_current;
	heap_current += size;
	return ptr;
}

void kfree(void* ptr)
{
	(void)ptr; // bump allocator
}