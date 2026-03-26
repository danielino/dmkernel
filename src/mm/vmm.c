#include "mm/vmm.h"

#include "kernel/utils/printk.h"
#include "mm/pmm.h"

// page directory e prima page table allineate a 4KB
static page_directory_t kernel_dir __attribute__((aligned(4096)));
static page_table_t kernel_table __attribute__((aligned(4096)));

void vmm_map_page(page_directory_t* dir, uint32_t virt, uint32_t phys, uint32_t flags)
{
	uint32_t dir_idx = virt >> 22;			   // primi 10 bit
	uint32_t table_idx = (virt >> 12) & 0x3FF; // secondi 10 bit

	// se la page table non esiste, creala
	if (!((*dir)[dir_idx] & PAGE_PRESENT)) {
		page_table_t* table = pmm_alloc_frame();
		(*dir)[dir_idx] = (uint32_t)table | PAGE_PRESENT | PAGE_WRITABLE;
	}

	page_table_t* table = (page_table_t*)((*dir)[dir_idx] & ~0xFFF);
	(*table)[table_idx] = phys | flags;
}

void vmm_init(void)
{
	// identity map primi 4MB (1024 pagine da 4KB)
	for (uint32_t i = 0; i < 1024; i++)
		kernel_table[i] = (i * PAGE_SIZE) | PAGE_PRESENT | PAGE_WRITABLE;

	kernel_dir[0] = (uint32_t)&kernel_table | PAGE_PRESENT | PAGE_WRITABLE;

	// carica page directory in CR3 e abilita paging in CR0
	asm volatile("mov %0, %%cr3\n"
				 "mov %%cr0, %%eax\n"
				 "or $0x80000000, %%eax\n"
				 "mov %%eax, %%cr0\n" ::"r"(&kernel_dir)
				 : "eax");

	printk(KERN_INFO "VMM: paging enabled\n");
}