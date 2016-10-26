#ifndef SHELL_H_
#define SHELL_H_

#include <commands.h>

#define MAX_USERNAME_SIZE 20


void init_shell(void);	
void update_shell(void);

void get_username(char * buffer);

command_t get_command(command_id id);

#endif