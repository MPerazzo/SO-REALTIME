#include <commands.h>
#include <syscalls.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <shell.h>
#include <fractals.h>
#include <tests.h>

static char * user = "USER";

static char username[MAX_USERNAME_SIZE];

int echo(int argc, char ** argv) {
	int i;
	char * arg;

	for (i = 0; i < argc; i++) {
		arg = argv[i];
		if (arg[0] == '$') {
			if (strcmp(user, arg + 1) == 0) {
				get_username(username);
				printf("%s ", username);
			}
		}
		else {
			printf("%s ", arg);
		}
	}

	putchar('\n');

	return 0;
}

int help(int argc, char ** argv) {
	if (argc > 1) {
		fputs(STDERR, "Too many arguments!\n");
		return -1;
	}

	if (argc == 1) {
		char * option = argv[0];
		command_t command;
		int i, found = 0;
			
		for (i = 0; i < COMMANDS_SIZE; i++) {
		 	command = get_command(i);
			if (strcmp(command.name, option) == 0) {
				printf("%s: %s\n", command.name, command.desc);
				found = 1;
				break;
			}
		}

		if (!found) {
			fprintf(STDERR, "Invalid argument: '%s'\n", option);
			return -1;	
		}

		switch (command.id) {
			case ECHO:	
				printf("\tWrite arguments to the standard output, separated by a single space.\n");
				break;
			case HELP:
				printf("\tDisplays information about commands.\n");
				printf("\tIf no command is specified, prints a list of available commands.\n");
				break;
			case FRACTAL:
				printf("\tStarts video mode and draws the specified fractal.\n");
				break;
			case CLEAR:
				printf("\tClears the console.\n");
				break;
			case EXIT:
				printf("\tExits the shell.\n");
				break;
			case TIME:
				printf("\tEnables/disables time in the top right corner of the shell.\n");
				break;
			case DATE:
				printf("\tPrints the date in the console.\n");
				break;
			case SET_TIME:
				printf("\tSets the time with the given parameters.\n");
				break;
			case SET_DATE:
				printf("\tSets the date with the given parameters.\n");
				break;
			case SLEEP:
				printf("\tSleeps for the given seconds.\n");
				break;
			case TESTS:
				printf("\tRuns a series of tests to show the functionality of the standar library.\n");
				break;
			case PRINTF:
				printf("\tWrites formatted output to the console.\n");
				break;
		}

		return 0;
	}

	int i;

	for (i = 0; i < COMMANDS_SIZE; i++) {
		command_t command = get_command(i);
		printf("%s\n", command.desc);
	}

	return 0;
}


int fractal(int argc, char ** argv) {
	if (argc > 1) {
		fputs(STDERR, "Too many arguments!\n");
		return -1;
	}

	if (argc == 0) {
		printf("Available fractals: \n");
		printf("\t1- Mandelbrot\n");
		printf("\t2- Julia Set\n");
		return 0;
	}

	char * option = argv[0];

	if (strcmp(option, "1") == 0) {

		while (1) {
			char name[5] = {0}; 
			int iter, r, g, b, r2, g2, b2;
			int args = fscanf(STDDATA, "%s %d %d %d %d %d %d %d", name, &iter, &r, &g, &b, &r2, &g2, &b2);

			if (args != 8) {
				fprintf(STDERR, "Wrong format!\n");
				return -1;
			}

			video(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_BPP);	// entra en modo video
			mandelbrot(iter, r, g, b, r2, g2, b2);
		}
	}
	else if (strcmp(option, "2") == 0) {
		video(DEFAULT_WIDTH, DEFAULT_HEIGHT, DEFAULT_BPP);	// entra en modo video
		juliaSet();
	}
	else {
		fprintf(STDERR, "Invalid option '%s'\n", option);
		return -1;
	}

	return 0;
}

int _clear(int argc, char ** argv) {
	if (argc != 0) {
		fputs(STDERR, "Too many arguments!\n");
		return -1;
	}
	clear();
	return 0;
}

int _exit(int argc, char ** argv) {
	if (argc != 0) {
		fputs(STDERR, "Too many arguments!\n");
		return -1;
	}
	return exit(0);
}

int _time(int argc, char ** argv) {
	if (argc != 0) {
		fputs(STDERR, "Too many arguments!\n");
		return -1;
	}
	time();
	return 0;
}

int _date(int argc, char ** argv) {
	if (argc != 0) {
		fputs(STDERR, "Too many arguments!\n");
		return -1;
	}
	char * d = date();
	printf("%s\n", d);
	return 0;
}

int _set_time(int argc, char ** argv) {
	if (argc != 3) {
		fprintf(STDERR, "Invalid number of arguments!\n");
		return -1;
	}

	int hour, minutes, seconds;
	
	if (sscanf(argv[0], "%d", &hour) == 1) {
		if (sscanf(argv[1], "%d", &minutes) == 1) {
			if (sscanf(argv[2], "%d", &seconds) == 1) {
				if (seconds < 0 || seconds >= 60) {
					fprintf(STDERR, "Invalid seconds!\n");
					return -1;
				}
				if (minutes < 0 || minutes >= 60) {
					fprintf(STDERR, "Invalid minutes!\n");
					return -1;	
				}
				if (hour < 0 || hour >= 24) {
					fprintf(STDERR, "Invalid hour!\n");
					return -1;
				}
				return set_time(hour, minutes, seconds);
			}
		}
	}

	fprintf(STDERR, "Invalid arguments!\n");
	return -1;
}

int _set_date(int argc, char ** argv) {
	if (argc != 3) {
		fprintf(STDERR, "Invalid number of arguments!\n");
		return -1;
	}

	int day, month, year;

	if (sscanf(argv[0], "%d", &day) == 1) {
		if (sscanf(argv[1], "%d", &month) == 1) {
			if (sscanf(argv[2], "%d", &year) == 1) {
				if (day <= 0 || day > 31) {
					fprintf(STDERR, "Invalid day!\n");
					return -1;
				}
				if (month <= 0 || month > 12) {
					fprintf(STDERR, "Invalid month!\n");
					return -1;
				}
				if (year < 0 || year >= 80) {
					fprintf(STDERR, "Invalid year (must be between 0 and 79)\n");
					return -1;
				}
				return set_date(day, month, year);
			}
		}
	}

	fprintf(STDERR, "Invalid arguments!\n");
	return -1;
}

int sleep(int argc, char ** argv) {
	if (argc != 1) {
		fprintf(STDERR, "Invalid number of arguments!\n");
		return -1;
	}

	int seconds;

	if (sscanf(argv[0], "%d", &seconds) == 1) {
		if (seconds < 0) {
			fprintf(STDERR, "Invalid seconds!\n");
			return -1;	
		}
		printf("%d\n", seconds);
		return wait(seconds * 1000);
	}

	fprintf(STDERR, "Invalid argument!\n");
	return -1;
}

int tests(int argc, char ** argv) {
	if (argc != 0) {
		fprintf(STDERR, "Invalid number of arguments!\n");
		return -1;
	}

	run_tests();
	return 0;
}

/* Las siguientes funciones corresponden a una implementacion de printf por comando (PARECIERA funcionar, puede tener bugs) */

static void merge(char * str) {
	char c1, c2;
	char * aux = str + 1;
	c1 = *str;
	c2 = *(str + 1);

	if (c1 == '\\') {
		if (c2 == 'n') {
			*str = '\n';
		}
		else if (c2 == 't') {
			*str = '\t';
		}
	}

	while (*(aux + 1)) {
		*aux = *(aux + 1);
		aux++;
	}

	*aux = 0;
}

static char * check_special_characters(char * fmt) {
	char c;
	char * ret = fmt;
	while ((c = *fmt) != 0) {
		if (c == '\\'){
			merge(fmt);
		}
		fmt++;
	}
	return ret;
}

int _printf(int argc, char ** argv) {
	if (argc == 0) {
		return 0;
	}
	if (argc == 1) {
		puts(check_special_characters(argv[0]));
		return 1;
	}

	char * fmt = argv[0];
	char option;
	int i = 1;
	int number;

	while ((*fmt) != 0) {
		switch (*fmt) {
			case '%':
				if (i >= argc) {
					fprintf(STDERR, "Invalid arguments\n");
					return -1;
				}
				option  = *(++fmt);
				if (option == 's') {
					puts(argv[i]);

				} else if (option == 'd'){
					if (sscanf(argv[i], "%d", &number) == 1) {
						printf("%d", number);
					}
					else {
						fprintf(STDERR, "%s: invalid number\n", argv[i]);
						return -1;
					}
				} else if (option == 'c') {
					putchar(argv[i][0]);
				} else {
					fprintf(STDERR, "Unsupported format: '%%c'", option);
					return -1;
				}
				i++;
				break;
			default:
				fmt = check_special_characters(fmt);
				putchar(*fmt);
				break;
		}
		fmt++;
	}

	return 1;
}
