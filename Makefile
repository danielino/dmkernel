CC = i686-elf-gcc
AS = nasm
ARCH = x86
LINKER = arch/$(ARCH)/linker.ld

CFLAGS = -ffreestanding -O2 -Wall -Wextra -m32 -Iinclude
LDFLAGS = -T $(LINKER) -nostdlib -m32

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
ISO_DIR = $(BUILD_DIR)/iso

SRC = $(shell find src -name "*.c")
OBJ = $(patsubst src/%.c,$(OBJ_DIR)/%.o,$(SRC))

BOOT_OBJ = $(OBJ_DIR)/boot.o

ASM_SRC = $(shell find src -name "*.asm")                                                                                                                                                     
ASM_OBJ = $(patsubst src/%.asm,$(OBJ_DIR)/%.o,$(ASM_SRC))

all: $(BUILD_DIR)/kernel.iso

# 🧱 crea cartelle automaticamente
$(OBJ_DIR)/%.o: src/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BOOT_OBJ): boot/boot.asm
	@mkdir -p $(OBJ_DIR)
	$(AS) -f elf32 $< -o $@

# Compila altri .asm files in src/                                                                                                                                                            
$(OBJ_DIR)/arch/x86/%.o: src/arch/x86/%.asm                                                                                                                                                   
	@mkdir -p $(dir $@)                                            
	$(AS) -f elf32 $< -o $@ 

# 🔗 linking
$(BUILD_DIR)/kernel.bin: $(BOOT_OBJ) $(ASM_OBJ) $(OBJ)
	$(CC) $(LDFLAGS) $^ -o $@

# 💿 ISO
$(BUILD_DIR)/kernel.iso: $(BUILD_DIR)/kernel.bin
	@mkdir -p $(ISO_DIR)/boot/grub
	cp $< $(ISO_DIR)/boot/kernel.bin
	echo 'set timeout=0' > $(ISO_DIR)/boot/grub/grub.cfg
	echo 'set default=0' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo 'menuentry "kernel" {' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '  multiboot /boot/kernel.bin' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '  boot' >> $(ISO_DIR)/boot/grub/grub.cfg
	echo '}' >> $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(ISO_DIR)

# ▶️ run
run: $(BUILD_DIR)/kernel.iso
	qemu-system-x86_64 -m 512 -cdrom $<

# 🧹 clean
clean:
	rm -rf $(BUILD_DIR)