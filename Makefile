NASM     := nasm
CXX      := x86_64-elf-g++
LD       := x86_64-elf-ld
TARGET   := boofix
BUILD    := build/x86_64
TMP      := build/tmp
SRC      := src
ISO      := $(BUILD)/$(TARGET).iso
GRUB_CFG := build/boot/grub/grub.cfg
LINKER   := build/linker.ld
INCLUDE := -I$(SRC)/libs

# ---------------- Sources ----------------
asm_sources := $(shell find $(SRC) -name '*.asm')
asm_objects := $(patsubst $(SRC)/%, $(TMP)/%, $(asm_sources:.asm=.o))

cpp_sources := $(shell find $(SRC) -name '*.cpp')
cpp_objects := $(patsubst $(SRC)/%, $(TMP)/%, $(cpp_sources:.cpp=.o))

all_objects := $(asm_objects) $(cpp_objects)

# ---------------- Default target ----------------
all: build

# ---------------- Assemble ASM ----------------
$(TMP)/%.o : $(SRC)/%.asm
	mkdir -p $(dir $@)
	$(NASM) -f elf64 $< -o $@

# ---------------- Compile C++ ----------------
$(TMP)/%.o : $(SRC)/%.cpp
	mkdir -p $(dir $@)
	$(CXX) $(INCLUDE) -ffreestanding -O2 -std=gnu++20 -c $< -o $@

# ---------------- Link kernel ----------------
$(TMP)/kernel.elf: $(all_objects)
	mkdir -p $(TMP)
	$(LD) -n -o $@ -T $(LINKER) $(all_objects)

# ---------------- Create ISO ----------------
$(ISO): $(TMP)/kernel.elf $(GRUB_CFG)
	mkdir -p $(BUILD)/iso/boot/grub
	cp $(TMP)/kernel.elf $(BUILD)/iso/boot/kernel.elf
	cp $(GRUB_CFG) $(BUILD)/iso/boot/grub/grub.cfg
	grub-mkrescue -o $@ $(BUILD)/iso
	rm -rf $(BUILD)/iso
	rm -rf $(TMP)

# ---------------- Build ----------------
.PHONY: build
build: $(ISO)

# ---------------- Clean ----------------
.PHONY: clean
clean:
	rm -rf $(BUILD)
	rm -rf $(TMP)