#include <keyboard.h>
#include <keyboard_map.h>
#include <io.h>

#include <video.h>

#define KEYBOARD_DATA_PORT		0x60
#define KEYBOARD_STATUS_PORT	0x64

#define BUFFER_SIZE 64

static void key_pressed(char keycode);
static int isalpha(char keycode);

static unsigned char kb_buffer[BUFFER_SIZE] = { EMPTY };
static int write_pos = 0;
static int read_pos = 0;

static int shift_on = 0;
static int caps_lock_on = 0;

void keyboard_handler(void) {
	unsigned char status;
	char keycode;

	status = read_port(KEYBOARD_STATUS_PORT);
	/* Lowest bit of status will be set if buffer is not empty */
	if (status & 0x01) {
		keycode = read_port(KEYBOARD_DATA_PORT);
		key_pressed(keycode);
	}
}

/* Stores the keys pressed in the buffer */
void key_pressed(char keycode) {

	/* Check special characters */
	switch (keycode) {
		case CAPS_LOCK:
			caps_lock_on = !caps_lock_on;
			return;
		case LS_PRESSED:
		case RS_PRESSED:
			shift_on = 1;
			return;
		case LS_RELEASED:	
		case RS_RELEASED:
			shift_on = 0;
			return;
	}

	if (!(keycode & KEY_RELEASED)) {
		//putnumber(keycode, RED);

		int shift = (isalpha(keycode) && caps_lock_on);

		if (shift_on) {
			shift = !shift;
		}

		unsigned char ascii = keyboard_map[shift][(int)keycode];

		if (ascii != 0) {
			kb_buffer[write_pos++] = ascii;

			if (write_pos == BUFFER_SIZE) {
				write_pos = 0;
			}
		}
	}
 
}

/* Returns the ASCCI code of the last key pressed */
unsigned char get_key(void) {
	unsigned char key = kb_buffer[read_pos];
	kb_buffer[read_pos++] = EMPTY;

	if (read_pos == BUFFER_SIZE) {
		read_pos = 0;
	}

	return key;
}

// int is_empty(void) {
// 	return (kb_buffer[read_pos] == EMPTY);
// }

int isalpha(char keycode) {
	return ((keycode >= Q && keycode <= P) || (keycode >= A && keycode <= L) || (keycode >= Z && keycode <= M));
}