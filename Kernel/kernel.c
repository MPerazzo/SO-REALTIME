#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <video.h>
#include <interrupts.h>
#include <syscalls.h>
#include <memlib.h>
#include <buddy.h>
#include <types.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const codeModuleAddress = (void*)0x400000;
static void * const dataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();

void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		codeModuleAddress,
		dataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();
	return getStackBase();
}

int main()
{	
	clear_screen();
	init_syscalls();
	init_interrupts();
	
	puts_at("Arqui OS", GREEN, 0, 0);

	// before tests remove aligment in memlib.c, so the prints fits in screen.
	// memtestKernel();
	// memtestUser();

	((EntryPoint)codeModuleAddress)();

	return 0;
}

void memtestKernel() {

	init_kheap();

	init_mem(32, true);	

	void * p1 = get_memblock(8, true);

	ncPrintHex(p1);

    ncNewline();

	ncPrintHex(get_memblock(16, true));

	ncNewline();

	ncPrintHex(get_memblock(4, true));

	ncNewline();

	void *p3 = get_memblock(4, true);

	ncPrintHex(p3);

	buddy_print(true);

	free_memblock(p1, true);

	buddy_print(true);

	free_memblock(p3, true);

	buddy_print(true);

	free_memblock(0x100, true);

	buddy_print(true);
}

void memtestUser() {

	init_uheap();

	init_mem(32, false);	

	// buddy_init(32);

	void * p3 = get_memblock(4, false);

	ncPrintHex(get_memblock(8, false));

    ncNewline();

	ncPrintHex(get_memblock(16, false));

	ncNewline();

	ncPrintHex(get_memblock(4, false));

	ncNewline();

	ncPrintHex(get_memblock(4, false));

	free(p3, false);

	buddy_print(false);

}
