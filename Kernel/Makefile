include Makefile.inc

KERNEL=kernel.bin
SOURCES=$(wildcard *.c) $(wildcard drivers/*.c) $(wildcard system/*.c) $(wildcard system/video/*.c) $(wildcard system/MemoryManagement/*.c) $(wildcard util/*.c) $(wildcard system/sharedMemory/*.c)
SOURCES_ASM=$(wildcard system/asm/*.asm)
OBJECTS=$(SOURCES:.c=.o)
OBJECTS_ASM=$(SOURCES_ASM:.asm=.o)
LOADERSRC=loader.asm

LOADEROBJECT=$(LOADERSRC:.asm=.o)
STATICLIBS=

all: $(KERNEL)

$(KERNEL): $(LOADEROBJECT) $(OBJECTS) $(STATICLIBS) $(OBJECTS_ASM)	
	$(LD) $(LDFLAGS) -T kernel.ld -o $(KERNEL) $(LOADEROBJECT) $(OBJECTS) $(OBJECTS_ASM) $(STATICLIBS)

%.o: %.c
	$(GCC) $(GCCFLAGS) -I./include -I./system/include -I./system/video/include -I./drivers/include -I./system/MemoryManagement/include -I./system/sharedMemory/include -I./util/include -c $< -o $@

%.o : %.asm
	$(ASM) $(ASMFLAGS) $< -o $@

$(LOADEROBJECT):
	$(ASM) $(ASMFLAGS) $(LOADERSRC) -o $(LOADEROBJECT)

clean:
	rm -rf system/*.o system/asm/*.o system/video/*.o drivers/*.o system/MemoryManagement/*.o util/*.o system/sharedMemory/*.o *.o *.bin

.PHONY: all clean
