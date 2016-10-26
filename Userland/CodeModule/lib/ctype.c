#include <ctype.h>

/* Reference: http://www.cplusplus.com/reference/cctype */

int isalnum(int c) {
	return isalpha(c) || isdigit(c);
}

int isalpha(int c) {
	return c >= 'A' && c <= 'z';
}

int isdigit(int c) {
	return c >= '0' && c <= '9';
}

int islower(int c) {
	return c >= 'a' && c <= 'z';
}

int isprint(int c) {
	return !iscntrl(c);
}

int isspace(int c) {
	return c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f' || c == '\r';
}

int isupper(int c) {
	return c >= 'A' && c <= 'Z';
}

int isxdigit(int c) {
	return isdigit(c) || (c >= 'A' && c <= 'F') || (c >= 'a' && c <= 'f');
}

int tolower(int c) {
	if (islower(c)) {
		return c;
	}
	return c + ('a' - 'A');
}

int toupper(int c) {
	if (isupper(c)) {
		return c;
	}
	return c - ('a' - 'A');
}

int iscntrl(int c) {
	return (c >= NULL && c <= US) || c == DEL;
}

int isgraph(int c) {
	return isprint(c) && c != ' ';
}

int ispunct(int c) {
	return isgraph(c) && !isalnum(c);
}

int isblank(int c) {
	return c == ' ' || c == '\t';
}