#include <stdio.h>
#include <string.h>
#include <syscalls.h>
#include <stdarg.h>
#include <ctype.h>

static int print_arg(int fd, va_list ap, char option);
static int print_hex(int fd, int hex);
static int print_dec(int fd, int dec);
static int print_oct(int fd, int oct);
static int print_bin(int fd, int bin);
static int num_to_base(unsigned int value, char * buffer, unsigned int base);

static int vfprintf(int fd, const char * fmt, va_list ap);

void putchar(char c) {
	fputc(STDOUT, c);
}

void fputc(int fd, char c) {
	write(fd, &c, 1);
}

int puts(char * str) {
	return fputs(STDOUT, str);
}

int fputs(int fd, char * str) {
	char c;
	int written = 0;
	while((c = *str++) != 0) {
		fputc(fd, c);
		written++;
	}
	return written;
}

int fputsn(int fd, char * str, int n) {
	return write(fd, str, n);
}

int printf(const char * fmt, ...) {
	va_list ap;
	int written = 0;

	va_start(ap, fmt);
	written += vfprintf(STDOUT, fmt, ap);
	va_end(ap);

	return written;
}

int fprintf(int fd, const char * fmt, ...) {
	va_list ap;
	int written = 0;

	va_start(ap, fmt);
	written += vfprintf(fd, fmt, ap);
	va_end(ap);

	return written;	
}

#define SUPPORTED_PRINTF_FORMATS "sxdobc"

static int vfprintf(int fd, const char * fmt, va_list ap) {
	int written = 0;
	char c;

	while ((c = *fmt++) != 0) {
		if (c == '%') {
			char option = *fmt;
			if (strchr(SUPPORTED_PRINTF_FORMATS, option) == NULL) {
				fputc(fd, '%');
				written++;
			}
			else {
				fmt++;
				written += print_arg(fd, ap, option);
			}

		}
		else {
			fputc(fd, c);
			written++;
		}
	}

	return written;
}

static int print_arg(int fd, va_list ap, char option) {
	int written = 0;

	switch (option) {
		case 's':
			written += fputs(fd, va_arg(ap, char *));
			break;
		case 'x':
			written += print_hex(fd, va_arg(ap, int));
			break;
		case 'd':
			written += print_dec(fd, va_arg(ap, int));
			break;
		case 'o':
			written = print_oct(fd, va_arg(ap, int));
			break;
		case 'b':
			written = print_bin(fd, va_arg(ap, int));
			break;
		case 'c':
			fputc(fd, va_arg(ap, int) & UCHAR_MAX);
			written = 1;
			break;
		default:
			break;		// error de formato
	}

	return written;
}

static int print_hex(int fd, int num) {
	int digits, written;
	char buffer[20];
	digits =  num_to_base(num, buffer, 16);
	written = fputsn(fd, buffer, digits);
	// fputc(fd, 'h');
	// written++;
	return written;
}

static int print_dec(int fd, int num) {
	int digits;
	int negative = 0;
	char buffer[20];
	if (num < 0) {
		fputc(fd, '-');
		negative = 1;
		num *= -1;
	}
	digits = num_to_base(num, buffer, 10);
	return fputsn(fd, buffer, digits) + negative;
}

static int print_oct(int fd, int num) {
	int digits;
	char buffer[20];
	digits =  num_to_base(num, buffer, 8);
	return fputsn(fd, buffer, digits);
}

static int print_bin(int fd, int num) {
	int digits;
	char buffer[20];
	digits =  num_to_base(num, buffer, 2);
	return fputsn(fd, buffer, digits);
}

static int num_to_base(unsigned int value, char * buffer, unsigned int base) {
	char *p = buffer;
	char *p1, *p2;
	int digits = 0;

	//Calculate characters for each digit
	do { 
		int remainder = value % base;
		*p++ = (remainder < 10) ? remainder + '0' : remainder + 'A' - 10;
		digits++;
	}
	while (value /= base);

	// Terminate string in buffer.
	*p = 0;

	//Reverse string in buffer.
	p1 = buffer;
	p2 = p - 1;
	while (p1 < p2)
	{
		char tmp = *p1;
		*p1 = *p2;
		*p2 = tmp;
		p1++;
		p2--;
	}

	return digits;
}

/*****************************************************/

static int vfscanf(int fd, const char * fmt, va_list ap);
static int vsscanf(const char * buffer, const char * fmt, va_list ap);

static int read_arg(const char * buffer, va_list ap, char option, char nextchar);
/* Reads until a space or the nextchar is found */
static int read_str(const char * buffer, char * ptr, char nextchar);
/* Reads until a non-digit character is found or the number is too big */
static int read_dec(const char * buffer, int * ptr);

int getchar() {
	return fgetc(STDIN);
}

int fgetc(int fd) {
	char c;
	read(fd, &c, 1);
	return c;
}

char * gets(char * str, int n) {
	return fgets(STDIN, str, n);
}

char * fgets(int fd, char * str, int n) {
	char c;
	int read = 0;

	while ((c = fgetc(fd)) != EOF && c != '\n' && read < n) {
		if (c > 0) {
			if (c == '\b') {
				if (read > 0) {
					if (fd == STDIN) {
						fputc(STDOUT, c);
					}
					str[--read] = 0;
				}
			}
			else {
				if (fd == STDIN) {
					fputc(STDOUT, c);
				}
				str[read++] = c;
			}
		}
	}

	if (fd == STDIN) {
		fputc(STDOUT, '\n');
	}

	if (read == 0 && c == EOF) {
		return NULL;
	}

	str[read] = 0;
	return str;
}

/* Based on code from http://mirror.fsf.org/pmon2000/3.x/src/lib/libc/scanf.c */
int scanf(const char * fmt, ...) {
	va_list ap;
	int read;

	va_start(ap, fmt);
	read = vfscanf(STDIN, fmt, ap);
	va_end(ap);

	return read;
}

int fscanf(int fd, const char * fmt, ...) {
	va_list ap;
	int read;

	va_start(ap, fmt);
	read = vfscanf(fd, fmt, ap);
	va_end(ap);

	return read;
}

int sscanf(const char * str, const char * fmt, ...) {
	int read;
	va_list ap;

	va_start(ap, fmt);
	read = vsscanf(str, fmt, ap);
	va_end(ap);

	return read;
}

#define MAX_SIZE 64

static int vfscanf(int fd, const char * fmt, va_list ap) {
	int read;
	char buffer[MAX_SIZE];

	if (fgets(fd, buffer, MAX_SIZE) == NULL) {
		return -1;
	}

	read = vsscanf(buffer, fmt, ap);

	return read;
}

#define SUPPORTED_SCANF_FORMATS "sdc"

static int vsscanf(const char * buffer, const char * fmt, va_list ap) {
	int read = 0;
	char * format  = (char *)fmt;

	while (*buffer && *fmt) {
		while (isspace(*fmt))
			fmt++;
		while (isspace(*buffer))
			buffer++;

		if (*fmt == '%') {
			char option = *(++fmt);
			int len;

			if (strchr(SUPPORTED_SCANF_FORMATS, option) == NULL) {
				fprintf(STDERR, "Unsupported format '%%c'\n", option);
				return read;
			}

			len = read_arg(buffer, ap, option, *(++fmt));
			if (len <= 0) {
				fprintf(STDERR, "Wrong format, expected '%%c'\n", option);
				return read;
			}
			buffer += len;
			read++;
		}
		else {
			if (*buffer != *fmt) {
				fprintf(STDERR, "Wrong format, expected character '%c'\n", *fmt);
				return read;
			}
			else {
				fmt++;
				buffer++;
			}
		}
	}

	while (isspace(*fmt))
		fmt++;
	while (isspace(*buffer))
		buffer++;

	if (*fmt != 0 || *buffer != 0) {
		fprintf(STDERR, "Wrong format, expected '%s'\n", format);
		return -1;
	}

	return read;
}

static int read_arg(const char * buffer, va_list ap, char option, char nextchar) {
	int read = 0;
	char * ch;

	switch (option) {
		case 's':
			read = read_str(buffer, va_arg(ap, char *), nextchar);
			break;
		case 'd':
			read = read_dec(buffer, va_arg(ap, int *));
			break;
		case 'c':
			read = 1;
			ch = va_arg(ap, char *);
			*ch = *buffer;
			break;
		default:
			break;
	}

	return read;
}

static int read_str(const char * buffer, char * ptr, char nextchar) {
	int read = 0;
	char c;

 	while (!isspace((c = buffer[read])) && c != 0 && c != EOF && c != nextchar) {
 		ptr[read++] = c;
 	}

 	ptr[read] = 0;
 	return read;
}

#define between(x, y, z) ((x) >= (y) && (x) <= (z))

static int read_dec(const char * buffer, int * ptr) {
	int number = 0;
	int digits = 0;
	int negative = 0;
	char c;

	if (*buffer == '-') {
		negative = 1;
		buffer++;
	}

 	while (!isspace((c = buffer[digits])) && c != 0 && c != EOF) {
		if (c < '0' || c > '9' || !between(number, 0, INT_MAX - 1)) {
			break;
		}
		number = number * 10 + c - '0';
		digits++;
	}

	if (negative) {
		number *= -1;
	}

	*ptr = number;
	return digits + negative;
}