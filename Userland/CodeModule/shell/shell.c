#include <stdio.h>
#include <shell.h>
#include <syscalls.h>
#include <string.h>
#include <commands.h>
#include <ctype.h>

#define BUFFER_SIZE 256

#define ignore_spaces(str) { while (isspace(*str)) str++; }
#define ignore_characters(str) { while (*str != 0 && !isspace(*str)) str++;}

static void init_commands(void);
static void add_command(command_id id, const char * name, const char * desc, command_fn fn);
static int parse_args(char ** argv, char * str);
static void execute_command(void);

static void _get_username(void);
static void clean_buffer(void);
static void prompt(void);
static int is_empty(void);

static char buffer[BUFFER_SIZE] = { 0 };
static int current_pos = 0;

static char username[MAX_USERNAME_SIZE];
static char * os_name = "os";

command_t commands[COMMANDS_SIZE];

void init_shell() {
	_get_username();
	clean_buffer();
	init_commands();
	putchar('\n');
	prompt();
}

void update_shell() {

	char key = getchar();

	if (key <= 0) return;

	switch (key) {
		case '\n':
			if (current_pos != 0) {
				putchar('\n');
			}
			execute_command();
			prompt();
			clean_buffer();
			break;
		case '\b':
			if (current_pos > 0) {
				putchar('\b');	
				buffer[--current_pos] = 0;
			}	
			break;
		case ' ':		// no imprimo ni agrego al buffer los espacios iniciales
		case '\t':
			if (current_pos == 0) {
				return;
			}
		default:
			putchar(key);
			if (current_pos == BUFFER_SIZE) {
				current_pos = 0;
			}
			buffer[current_pos++] = key;
			break;
	}
}

void _get_username(void) {
	int i;
	char c;

	do {
		i = 0;
		clean_buffer();
		printf("Please enter your username (up to %d characters): ", MAX_USERNAME_SIZE);
		while ((c = getchar()) != '\n') {
			if (c == '\b') {
				if (i > 0) {
					putchar('\b');
					buffer[--i] = 0;
				}
			}
			else if (c > 0 && !isspace(c)) {
				putchar(c);
				buffer[i++] = c;
			}
		}
		if (i == 0) {
			putchar('\n');
		}
		else if (i > MAX_USERNAME_SIZE) {
			fprintf(STDERR, "\nToo long...\n");
		}
	} while (i <= 0 || i > MAX_USERNAME_SIZE);

	strcpy(buffer, username);
}

void init_commands(void) {
	add_command(ECHO, "echo", "echo [args...]", echo);
	add_command(HELP, "help", "help [command]?", help);
	add_command(FRACTAL, "fractal", "fractal [number]", fractal);
	add_command(CLEAR, "clear", "clear", _clear);
	add_command(EXIT, "exit", "exit", _exit);
	add_command(TIME, "time", "time", _time);
	add_command(DATE, "date", "date", _date);
	add_command(SET_TIME, "settime", "settime [hour] [minutes] [seconds]", _set_time);
	add_command(SET_DATE, "setdate", "setdate [day] [month] [year]", _set_date);
	add_command(SLEEP, "sleep", "sleep [seconds]", sleep);
	add_command(TESTS, "tests", "tests", tests);
	add_command(PRINTF, "printf", "printf [format] [args...]", _printf);
}

void add_command(command_id id, const char * name, const char * desc, command_fn fn) {
	commands[id].id = id;
	commands[id].name = name;
	commands[id].desc = desc;
	commands[id].fn = fn;
}

void execute_command() {
	int i = 0;

	if (is_empty()) {
		putchar('\n');
		return;
	}

	for (i = 0; i < COMMANDS_SIZE; i++) {
		command_t command =  commands[i];
		int len = prefix(command.name, buffer);

		if (len > 0) {
			char * argv[MAX_ARGS];
			char * args_start = buffer + len;
			int argc;

			if (*args_start != '\0' && !isspace(*args_start)) {	//	habia otro caracter luego del comando, sin separacion
				break;
			}

			argc = parse_args(argv, args_start);

			command.fn(argc, argv);
			return;
		}
	}

	fprintf(STDERR, "Invalid command!\n");
}

int parse_args(char ** argv, char * str) {
	int i = 0;
	char * arg;
	
	ignore_spaces(str);
	while (*str != NULL) {
		arg = str;
		ignore_characters(str);
		*str++ = 0;
		argv[i++] = arg;
		if (i == MAX_ARGS) {
			return i;
		}
		ignore_spaces(str);
	}

	return i;
}

void clean_buffer() {
	int i;
	for (i = 0; i < BUFFER_SIZE; i++) {
		buffer[i] = 0;
	}
	current_pos = 0;
}

void prompt() {
	printf("%s@%s> ", username, os_name);
}

int is_empty() {
	return buffer[0] == 0;
}

void get_username(char * buffer) {
	strcpy(username, buffer);
}

command_t get_command(command_id id) {
	return commands[id];
}