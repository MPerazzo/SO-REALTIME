#ifndef VIDEO_H_
#define VIDEO_H_

#define BLACK 			0x00
#define BLUE			0x01
#define GREEN			0x02
#define CYAN			0x03
#define RED				0x04
#define MAGENTA			0x05
#define BROWN			0x06
#define LIGHT_GREY		0x07
#define DARK_GREY		0x08
#define LIGHT_BLUE		0x09
#define LIGHT_GREEN		0x0A
#define LIGHT_CYAN		0x0B
#define LIGHT_RED		0x0C
#define LIGHT_MAGENTA	0x0D
#define LIGHT_BROWN		0x0E
#define WHITE			0x0F

#define DEFAULT	LIGHT_GREY

#define INDEX_OUT_OF_BOUNDS_ERROR -1

void clear_screen(void);
void clear_console(void);
void fill(char ch, char attr);

/* Prints at screen[r][c] */
int putnumber_at(int n, char attr, int r, int c);
int putchar_at(char ch, char attr, int r, int c);
int puts_at(char *str, char attr, int r, int c);

/* Prints to the current location of the screen */
void putnumber(int n, char attr);
void putchar(char ch, char attr);
void puts(char *str, char attr);

void hide_cursor(void);

#endif