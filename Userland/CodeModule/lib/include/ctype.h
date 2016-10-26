#ifndef CTYPES_H
#define CTYPES_H

#ifndef NULL
#define NULL 	0x00
#endif

#define BS		0x08	/* '\b' backspace */
#define US 		0x1F	/* unit separator */
#define SPC		0x20	/* ' ' space */
#define DEL		0x7F	/* delete */

#define TAB '\t'	/* 0x09 horizontal tab*/
#define LF	'\n'	/* 0x0A newline */
#define VT 	'\v'	/* 0x0B vertical tab */
#define FF	'\f'	/* 0x0C feed */
#define CR 	'\r'	/* 0x0D carriage return */

int isalnum(int c);
int isalpha(int c);
int isdigit(int c);
int islower(int c);
int isprint(int c);
int isspace(int c);
int isupper(int c);
int isxdigit(int c);

int tolower(int c);
int toupper(int c);

//This function checks whether the passed character has graphical representation.
int isgraph(int c);
//This function checks whether the passed character is a punctuation character.
int ispunct(int c);
//This function checks whether the passed character is control character.
int iscntrl(int c);
// Check if the passed character is ' ' or '\t'
int isblank(int c);

#endif