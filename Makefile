ASM=nasm
CC=$(HOME)/opt/cross/bin/i686-elf-gcc

SRC_DIR=src
BUILD_DIR=build
ISO_DIR=$(BUILD_DIR)/iso

INCLUDE_DIR=$(SRC_DIR)/kernel/include

KERNEL_ASM		:= $(shell find $(SRC_DIR)/kernel -name "*.asm")
KERNEL_C		:= $(shell find $(SRC_DIR)/kernel -name "*.c")

KERNEL_ASM_O	:= $(patsubst $(SRC_DIR)/kernel/%.asm, $(BUILD_DIR)/kernel/%.asm.o, $(KERNEL_ASM))
KERNEL_C_O		:= $(patsubst $(SRC_DIR)/kernel/%.c, $(BUILD_DIR)/kernel/%.c.o, $(KERNEL_C))

KERNEL 			= $(BUILD_DIR)/clipper.bin
IMAGE			= $(BUILD_DIR)/clipper.iso

.PHONY: all clipper kernel clean always

clipper: kernel
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(KERNEL) $(ISO_DIR)/boot/clipper.bin
	cp grub/grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(IMAGE) $(ISO_DIR)

kernel: $(KERNEL_ASM_O) $(KERNEL_C_O)
	$(CC) -T linker.ld -o $(KERNEL) -ffreestanding -O2 -nostdlib $^ -lgcc

$(BUILD_DIR)/%.asm.o: $(SRC_DIR)/%.asm
	@mkdir -p "$(@D)"
	$(ASM) -felf32 $< -o $@

$(BUILD_DIR)/%.c.o: $(SRC_DIR)/%.c
	@mkdir -p "$(@D)"
	$(CC) -c $< -o $@ -I$(INCLUDE_DIR) -std=gnu99 -ffreestanding -O2 -Wall -Wextra -Wformat

clean:
	rm -rf $(BUILD_DIR)/*
