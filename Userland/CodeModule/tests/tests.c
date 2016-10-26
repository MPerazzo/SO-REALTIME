#include <tests.h>

#define ERROR_MESSAGE "TEST FAILED"
#define CLEAN_BUFFER while (getchar() != '\n');

static void run_test(void (*test)(void));

void run_tests() {
	run_test(printf_test);
	run_test(scanf_test);
	run_test(fgets_test);
	run_test(malloc_test);
	puts("Type enter to continue...");
	CLEAN_BUFFER;
	clear();
}

static void run_test(void (*test)(void)) {
	puts("\nType enter to start the next test...\n");
	CLEAN_BUFFER;
	puts("*******************************\n");
	test();
	puts("*******************************\n");
}

void printf_test() {
	int i;
	puts("printf test:\n");
	i = printf("%s\n", "string");
	printf("characters written: %d\n", i);
	assert(i == 7, ERROR_MESSAGE);
	i = printf("%x\n", 10);
	printf("characters written: %d\n", i);
	assert(i == 2, ERROR_MESSAGE);
	i = printf("%d\n", -10);
	printf("characters written: %d\n", i);
	assert(i == 4, ERROR_MESSAGE);
	i = printf("%o\n", 10);
	printf("characters written: %d\n", i);
	assert(i == 3, ERROR_MESSAGE);
	i = printf("%b\n", 10);
	printf("characters written: %d\n", i);
	assert(i == 5, ERROR_MESSAGE);
	i = printf("%c %c\n", 'A', 'A' + UCHAR_MAX + 1);
	printf("characters written: %d\n", i);
	assert(i == 4, ERROR_MESSAGE);
}

void scanf_test() {
	int i, number = 0;
	char str[64] = {0};
	char c = 0;
	char * fmt = "start-%c-%s-%d-end";

	puts("scanf test:\n");

	printf("Type a text with the following format (ignore spaces): '%s' \n", fmt);
	i = scanf(fmt, &c , str, &number);
	printf("You typed: %c, %s, %d\n", c, str, number);
	assert(i == 3, ERROR_MESSAGE);

}

void fgets_test() {
	char buffer[50] = {0};

	puts("fgets test:\n");
	puts("Reading from data module...\n");


	while (fgets(STDDATA, buffer, 50) != NULL) {
		printf("%s\n", buffer);
	}

	// char c;
	// int i = 0;
	// while ((c = fgetc(STDDATA)) != EOF) {
	//  	buffer[i++] = c;
	//  	putchar(c);
	// }
}

void malloc_test() {
	char * ptr;
	unsigned long address;

	puts("malloc test:\n");

	ptr = malloc(4);
	printf("ptr = %d\n", ptr);

	address = (unsigned long)ptr;
	ptr = malloc(10);
	assert((unsigned long)ptr == (address + 4), ERROR_MESSAGE);

	printf("ptr = %d\n", ptr);

	address = (unsigned long)ptr;
	ptr = malloc(0);
	assert((unsigned long)ptr == (address + 10), ERROR_MESSAGE);

	printf("ptr = %d\n", ptr);
}

bool assert(bool condition, const char * msg) {
	if (!condition) {
		fprintf(STDERR, "%s\n", msg);
		return false;
	}
	return true;
}