#ifndef STRING_H
#define STRING_H

int strlen(const char * str);
int strcpy(const char * from, char * to);
int strncpy(const char * from, char * to, int n);
int strcmp(const char * s1, const char * s2);
int strncmp(const char * s1, const char * s2, int n);

/* Returns a pointer to the first occurrence of c in str, or NULL if not present*/
char * strchr(const char * str, char c);
/* Returns a pointer to the first occurrence of str2 in str1, or NULL if not present*/
char * strstr(const char * str1, const char * str2);

/* Returns the length of str1 if str1 is prefix of str2, else returns 0 */
int prefix(const char * str1, const char * str2);

#endif