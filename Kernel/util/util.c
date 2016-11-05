#include "util.h"

int strlen(char* s) {
	int l = 0;
	while (*s != 0) {
		l++;
		s++;
	}
	return l;
}

int strcmp(char* s1, char* s2) {

  while (*s1 != '\0' || *s2 != '\0') {
    if (*s1 != *s2) {
      return *s1-*s2;
    }
    s1++;
    s2++;
  }
  if (*s1 != '\0')
    return 1;
  if (*s2 != '\0')
    return -1;
  return 0;
}

char* strcpy(char *dst, char* src) {
  
  while (*src != '\0') {
    *dst = *src;
    dst++;
    src++;
  }

  *dst = '\0';

  return dst;
}
