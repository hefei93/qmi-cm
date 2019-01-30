ifneq ($(CROSS_COMPILE),)
CROSS-COMPILE:=$(CROSS_COMPILE)
endif
#CROSS-COMPILE:=/workspace/buildroot/buildroot-qemu_mips_malta_defconfig/output/host/usr/bin/mips-buildroot-linux-uclibc-
#CROSS-COMPILE:=/workspace/buildroot/buildroot-qemu_arm_vexpress_defconfig/output/host/usr/bin/arm-buildroot-linux-uclibcgnueabi-
#CROSS-COMPILE:=/workspace/buildroot-git/qemu_mips64_malta/output/host/usr/bin/mips-gnu-linux-
CC:=$(CROSS-COMPILE)gcc
LD:=$(CROSS-COMPILE)ld

BIN=serial
INCLUDE = include
CPPS += error.c
CPPS += log.c
CPPS += serial.c
CPPS += options.c
CPPS += setspeed2.c
CPPS += time.c
CPPS += tty.c

all: clean
	$(CC) $(CPPS) -o $(BIN) -I ./include
clean:
	rm -rf serial