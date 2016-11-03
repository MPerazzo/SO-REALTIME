#include <syscalls.h>
#include <video.h>
#include <keyboard.h>
#include <bga.h>
#include <timer.h>
#include <rtc.h>
#include <memlib.h>
#include <process.h>

extern void haltcpu(void);

static uint64_t sys_exit(uint64_t code, uint64_t arg2, uint64_t arg3);
static uint64_t sys_clear(uint64_t arg1, uint64_t arg2, uint64_t arg3);
static uint64_t sys_read(uint64_t fd, uint64_t buffer, uint64_t len);
static uint64_t sys_write(uint64_t fd, uint64_t buffer, uint64_t len);
static uint64_t sys_video(uint64_t width, uint64_t height, uint64_t bpp);
static uint64_t sys_draw(uint64_t x, uint64_t y, uint64_t color);
static uint64_t sys_sbrk(uint64_t increment, uint64_t arg2, uint64_t arg3);
static uint64_t sys_memblock(uint64_t size, uint64_t arg2, uint64_t arg3);
static uint64_t sys_freeblock(uint64_t block_pointer, uint64_t arg2, uint64_t arg3);
static uint64_t sys_pinitheap(uint64_t process, uint64_t arg2, uint64_t arg3);
static uint64_t sys_malloc(uint64_t process, uint64_t size, uint64_t arg3);
static uint64_t sys_calloc(uint64_t process, uint64_t amount, uint64_t size);
static uint64_t sys_free(uint64_t process, uint64_t mem, uint64_t arg3);
static uint64_t sys_time(uint64_t arg1, uint64_t arg2, uint64_t arg3);
static uint64_t sys_date(uint64_t arg1, uint64_t arg2, uint64_t arg3);
static uint64_t sys_set_time(uint64_t hour, uint64_t minutes, uint64_t seconds);
static uint64_t sys_set_date(uint64_t day, uint64_t month, uint64_t year);
static uint64_t sys_wait(uint64_t millis, uint64_t arg2, uint64_t arg3);

static uint64_t read_stdin(char * buffer, int len);
static uint64_t read_stddata(char * buffer, int len);

static SYSCALL syscalls[SYSCALLS_SIZE];		// array de punteros a funcion para las syscalls

/* Called when instruction int 80 is executed */
uint64_t syscallDispatcher(uint64_t rdi, uint64_t rsi, uint64_t rdx, uint64_t rcx) {
	if (rdi < SYS_EXIT || rdi > SYS_WAIT) {
		return 0;
	}
	return syscalls[rdi](rsi, rdx, rcx);
}

/* Initializes software interrupts (syscalls) */
void init_syscalls() {
	syscalls[SYS_EXIT] = sys_exit;
	syscalls[SYS_CLEAR] = sys_clear;
	syscalls[SYS_READ] = sys_read;
	syscalls[SYS_WRITE] = sys_write;
	syscalls[SYS_VIDEO] = sys_video;
	syscalls[SYS_DRAW] = sys_draw;
	syscalls[SYS_SBRK] = sys_sbrk;
	syscalls[SYS_MEMBLOCK] = sys_memblock;
	syscalls[SYS_FREEBLOCK] = sys_freeblock;

	syscalls[SYS_TIME] = sys_time;
	syscalls[SYS_DATE] = sys_date;
	syscalls[SYS_SET_TIME] = sys_set_time;
	syscalls[SYS_SET_DATE] = sys_set_date;
	syscalls[SYS_WAIT] = sys_wait;
}

/* Finishes the execution of the system */
uint64_t sys_exit(uint64_t code, uint64_t arg2, uint64_t arg3) {
	clear_screen();
	hide_cursor();
	puts_at("It is now safe to turn off your computer", DEFAULT, 0, 0);
	haltcpu();
	return code;
}

/* Clears the console */
uint64_t sys_clear(uint64_t arg1, uint64_t arg2, uint64_t arg3) {
	clear_console();
	return 0;
}

/* Writes to the given file descriptor and returns how many bytes were written */
uint64_t sys_write(uint64_t fd, uint64_t buf, uint64_t size) {
	char * buffer = (char *)buf;
	int len = size;
	char attr;

	switch(fd) {
		case STDOUT:
			attr = LIGHT_GREY;
			break;
		case STDERR:
			attr = RED;
			break;
		default:
			return 0;
	}

	while(len--  && *buffer != 0) {
		putchar(*buffer, attr);
		buffer++;
	}

	return size - len - 1;
}


/* Reads from the given file descriptor and returns how many bytes were read */
uint64_t sys_read(uint64_t fd, uint64_t buf, uint64_t size) {
	char * buffer = (char *)buf;

	switch (fd) {
		case STDIN:
			return read_stdin(buffer, size);
		case STDDATA:
			return read_stddata(buffer, size);
		default:  
			return 0;
	}
}

extern void _sti(void);

uint64_t read_stdin(char * buffer, int len) {
	int i = 0;
	unsigned char c;

	_sti();

	while (i < len) {
		c = get_key();
		if (c != EMPTY) {
			buffer[i++] = c;
		}
	}

	return i;
}

static void * const dataModuleAddress = (void*)0x500000;

uint64_t read_stddata(char * buffer, int len) {
	static int read_index = 0;
	char * dir = (char *)(dataModuleAddress + read_index);
	int i = 0;

	while (i < len && *dir != 0) {
		buffer[i++] = *dir++;
		read_index++;	
	}
	
	if (*dir == EOF || i == 0) {
		read_index = 0;
		buffer[i] = EOF;
	}

	return i;
}

static int video_mode_enabled = 0;

/* Starts VESA video mode with the given parameters */
uint64_t sys_video(uint64_t width, uint64_t height, uint64_t bpp) {
	if (!video_mode_enabled) {
		puts("Starting video mode...", LIGHT_GREY);	
		BgaSetVideoMode(width, height, 24, 1, 1);	// solo se acepta 24 bpp
		BgaFillScreen(0, 0, 0);
		video_mode_enabled = 1;
	}

	return 0;
}

/* Draws the pixel in the [x,y] position with the specified color. Returns true if the given position was valid */
uint64_t sys_draw(uint64_t x, uint64_t y, uint64_t color) {
	uint8_t r = (color >> 16) & 0xFF;
	uint8_t g = (color >> 8) & 0xFF;
	uint8_t b = color & 0xFF;

	return BgaDrawPixel(x, y, r, g, b);
}

static void * MEM_START_HEAP = (void *)0x600000;

/* Increments the data segment and returns the new top. NOTE: if increment is 0 then returns the old one */
uint64_t sys_sbrk(uint64_t increment, uint64_t arg2, uint64_t arg3) {
	MEM_START_HEAP += increment;
	return (uint64_t)MEM_START_HEAP;
}

/* Reserves a block of memory and returns a pointer to the beginning of it */
uint64_t sys_memblock(uint64_t size, uint64_t arg2, uint64_t arg3) {
	return (uint64_t)get_memblock(size);
}

/* Release the memory reserved previously by sys_memblock */
uint64_t sys_freeblock(uint64_t block_pointer, uint64_t arg2, uint64_t arg3) {
	return free_memblock((void *)block_pointer);
}

/* Initializes the heap of the process that is running */
uint64_t sys_pinitheap(uint64_t arg1, uint64_t arg2, uint64_t arg3) {
	return init_process_heap(current_process());
}

/* Allocates memory inside a block reserved previously by sys_memblock */
uint64_t sys_malloc(uint64_t size, uint64_t arg2, uint64_t arg3) {
	return (uint64_t)malloc(current_process(), size);
}

/* Similar to malloc but fills the memory that is being allocated with ceros */
uint64_t sys_calloc(uint64_t amount, uint64_t size, uint64_t arg3) {
	return (uint64_t)calloc(current_process(), amount, size);
}

/* Releases the memory that is being pointed by mem, which was previously reserved by calloc or malloc */
uint64_t sys_free(uint64_t mem, uint64_t arg2, uint64_t arg3) {
	return free(current_process(), (void *)mem);
}

/* Shows/hides the clock in the top right corner of the screen, returns a string representation of the time with the format hh:mm:ss */
uint64_t sys_time(uint64_t arg1, uint64_t arg2, uint64_t arg3) {
	if (remove_task((uint64_t)show_time)) {
		hide_time();
	}
	else {
		show_time();
		add_task(1, (uint64_t)show_time, true);
	}
	char * _time = time();
	return (uint64_t)_time;
}

/* Returns the date as a string */
uint64_t sys_date(uint64_t arg1, uint64_t arg2, uint64_t arg3) {
	char * _date = date();
	return (uint64_t)_date;
}

/* Sets the time to the given parameters */
uint64_t sys_set_time(uint64_t hour, uint64_t minutes, uint64_t seconds) {
	if (hour > 23 || minutes > 59 || seconds > 59) {
		return false;
	}
	set_time(hour, minutes, seconds);
	return true;
}

/* Sets the date to the given parameters */
uint64_t sys_set_date(uint64_t day, uint64_t month, uint64_t year) {
	if (day > 31 || month > 12 || year > 99) {
		return false;
	} 
	set_date(day, month, year);
	return true;
}

/* Sleeps for the given milliseconds */
uint64_t sys_wait(uint64_t millis, uint64_t arg2, uint64_t arg3) {
	wait(millis);
	return 0;
}





