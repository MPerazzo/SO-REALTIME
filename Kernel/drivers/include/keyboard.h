#ifndef KEYBOARD_H
#define KEYBOARD_H

#define EOF 0xFF

#define EMPTY 0xFF		//255

void keyboard_handler(void);
unsigned char get_key(void);
//int is_empty(void);

#endif