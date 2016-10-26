#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int strlen(const char * str) {
	int i = 0;
	while (*str++) { i++; }
	return i;
}

int strcmp(const char * s1, const char * s2) {

	if(*s1 == 0) {
		return *s2 != 0 ? -1 : 0;
	}
	if(*s2 == 0) {
		return 1;
	}

	int cmp = (*s1 > *s2)? 1 : ((*s1 < *s2)? -1 : 0);

	if(cmp != 0) {
		return cmp;
	}

	return strcmp(++s1, ++s2);
}

// TODO hacerlo iterativo
int strncmp(const char * s1, const char * s2, int n) {
	if (n == 0) {
		return 0;
	}	

	if(*s1 == 0) {
		return *s2 != 0 ? -1 : 0;
	}
	if(*s2 == 0) {
		return 1;
	}

	int cmp = (*s1 > *s2)? 1 : ((*s1 < *s2)? -1 : 0);

	if(cmp != 0) {
		return cmp;
	}

	return strncmp(++s1, ++s2, --n);
} 

int strcpy(const char * from, char * to) {
	while(*from) {
		*to = *from;
		to++;
		from++;
	}
	*to = 0;
	return 0;
}

int strncpy(const char * from, char * to, int n) {
	while(*from && n--) {
		*to = *from;
		to++;
		from++;
	}
	*to = 0;
	return 0;
}

char * strchr(const char * str, char c) {
	char * ret = (char *)str;

	while (*ret != 0 && *ret != c) ret++;

	if (*ret == 0) {
		return NULL;
	}

	return ret;
}

char * strstr(const char * str1, const char * str2) {
	char * aux = (char *)str1;
	char * ret = (char *)str2;
	char * start = ret;
	char c1, c2;

	str1++;

	while ((c1 = *aux++) != NULL && (c2 = *ret++) != NULL) {
		if (c1 != c2) {
			ret = ++start;
			aux = (char *)str1; // start over again
		}
	}

	if (c2 == NULL) {
		return NULL;
	}

	return start;
}

/* Returns the lenght of str1 if str1 is prefix of str2, else returns 0 */
int prefix(const char * str1, const char * str2) {
	int i = 0;

	while (str1[i] != 0) {
		if (str2[i] == 0 || str1[i] != str2[i]) {
			return 0;
		}
		i++;
	}

	return i;
}