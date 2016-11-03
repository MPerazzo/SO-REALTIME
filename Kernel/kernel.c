#include <stdint.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <video.h>
#include <interrupts.h>
#include <syscalls.h>
#include <memlib.h>
#include <buddy.h>
#include <process.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const codeModuleAddress = (void*)0x400000;
static void * const dataModuleAddress = (void*)0x500000;

process_t testingprocess;
process_t * current_process();

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

	init_mem(1024*4*4);

	//buddytest();

	//processmem_test();

	((EntryPoint)codeModuleAddress)();

	return 0;
}

void buddytest() {

	// test without aligment, as buddy.c functions are used instead of memlibs.


	buddy_init(32);

	ncPrintHex(0x600000 + buddy_alloc(8));

    ncNewline();

	ncPrintHex(0x600000 + buddy_alloc(16));

	ncNewline();

	ncPrintHex(0x600000 + buddy_alloc(4));

	ncNewline();

	ncPrintHex(0x600000 + buddy_alloc(4));

	buddy_print();

}

void processmem_test() {

	// test heap allocs, using aligment

	void * p1;

	p1 = get_memblock(1024);

	ncPrintHex(p1);

	testingprocess.heap_start = p1;

	testingprocess.heap_size = 4096;

	init_process_heap(current_process());

    ncNewline();


    void * p2;

	ncPrintHex(p2=malloc(current_process(), 1000));

    ncNewline();

    void * p3;

	ncPrintHex(p3=malloc(current_process(), 1000));

    ncNewline();

    void * p4;

	ncPrintHex(p4=malloc(current_process(), 1000));

    ncNewline();

    void * p5;

	ncPrintHex(p5=malloc(current_process(), 100));

    ncNewline();

    void * p6;

	ncPrintHex(p6=malloc(current_process(), 100));

	ncNewline();

	void * p7;

	ncPrintHex(p7=malloc(current_process(), 1000));

	ncNewline();

	free(current_process(), p3);

	ncPrintHex(malloc(current_process(), 1000));
}


// this function is for testing processes, scheduler will tell which process is the current.
process_t * current_process() {
	return &testingprocess;
}

