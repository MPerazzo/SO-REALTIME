#ifndef STDIO_H
#define STDIO_H

#include <stdarg.h>

#define EOF -1

#define UCHAR_MAX 	0xFF
#define INT_MIN		-2147483648
#define INT_MAX		2147483647
#define UINT_MAX	4294967295

#define NULL 0

#define STDIN	0
#define STDOUT	1
#define STDERR	2
#define STDDATA 3

/* Writes the character to STDOUT */
void putchar(char c);
/* Writes the character to the specified file descriptor */
void fputc(int fd, char c);
/* Writes the string to STDOUT */
int puts(char * str);
/* Writes the string to the specified file descriptor */
int fputs(int fd, char * str);
/* Writes the string or n characters if n is less than the lenght of the string */
int fputsn(int fd, char * str, int n);
/* Writes formatted output to STDOUT */
int printf(const char * fmt, ...);
/* Writes formatted output to the specified file descriptor */
int fprintf(int fd, const char * fmt, ...);

/* Reads a character from STDIN */
int getchar(void);
/* Reads a character from a file descriptor */
int fgetc(int fd);
/* Reads a line (until newline) or n characters from STDIN */
char * gets(char * str, int n);
/* Reads a line or n characters from the specified file descriptor */
char * fgets(int fd, char * str, int n);
/* Reads formatted input from STDIN */
int scanf(const char * fmt, ...);
/* Reads formatted input from the specified file descriptor */
int fscanf(int fd, const char * fmt, ...);
/* Reads formatted input from a string */
int sscanf(const char * str, const char * fmt, ...);

#endif