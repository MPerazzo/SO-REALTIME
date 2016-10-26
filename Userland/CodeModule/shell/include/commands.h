#ifndef COMMAND_H_
#define COMMAND_H_

#define MAX_ARGS 10
#define COMMANDS_SIZE 12

typedef enum {
	ECHO,
	HELP,
	FRACTAL,
	CLEAR,
	EXIT,
	TIME,
	DATE,
	SET_TIME,
	SET_DATE,
	SLEEP,
	TESTS, 
	
	PRINTF,			// version limitada de la funcion printf
} command_id;

typedef int (*command_fn)(int, char **);

typedef struct {
	command_id id;
	const char * name;	// string to identify the command when parsing
	const char * desc;	// brief description on how to use the command
	command_fn fn;	// funcion de ejecucion del comando
} command_t;

int echo(int argc, char ** argv);
int help(int argc, char ** argv);
int fractal(int argc, char ** argv);
int _clear(int argc, char ** argv);
int _exit(int argc, char ** argv);
int _time(int argc, char ** argv);
int _date(int argc, char ** argv);
int _set_time(int argc, char ** argv);
int _set_date(int argc, char ** argv);
int sleep(int argc, char ** argv);
int _printf(int argc, char ** argv);
int tests(int argc, char ** argv);

#endif
