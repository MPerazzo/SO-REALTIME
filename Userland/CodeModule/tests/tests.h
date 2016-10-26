#ifndef TESTS_H_
#define TESTS_H_

#include <syscalls.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

bool assert(bool condition, const char * msg);

void run_tests(void);
void printf_test(void);
void scanf_test(void);
void fgets_test(void);
void malloc_test(void);

#endif