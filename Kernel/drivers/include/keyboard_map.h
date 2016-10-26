#ifndef KEYBOARD_MAP_H_
#define KEYBOARD_MAP_H_


#define KEY_RELEASED  128

#define CTRL 29
#define LS_PRESSED 54
#define RS_PRESSED 42
#define LS_RELEASED -86
#define RS_RELEASED -74
#define CAPS_LOCK 58
#define Q 16
#define P 25
#define A 30
#define L 38
#define Z 44
#define M 50

#define UP_ARROW 72
#define DOWN_ARROW 80
#define LEFT_ARROW 75
#define RIGHT_ARROW 77


/* The following array is taken from 
    http://www.osdever.net/bkerndev/Docs/keyboard.htm
   All credits where due
*/

/* map[0] = lower characters, map[1] = upper characters */
unsigned char keyboard_map[2][128] =
{
    {
        0,  27, '1', '2', '3', '4', '5', '6', '7', '8',	/* 9 */
      '9', '0', '-', '=', '\b',	/* Backspace */
      '\t',			/* Tab */
      'q', 'w', 'e', 'r',	/* 19 */
      't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
        0,			/* 29   - Control */
      'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',	/* 39 */
     '\'', '`',   0,		/* Left shift */
     '\\', 'z', 'x', 'c', 'v', 'b', 'n',			/* 49 */
      'm', ',', '.', '/',   0,				/* Right shift */
      '*',
        0,	/* Alt */
      ' ',	/* Space bar */
        0,	/* Caps lock */
        0,	/* 59 - F1 key ... > */
        0,   0,   0,   0,   0,   0,   0,   0,
        0,	/* < ... F10 */
        0,	/* 69 - Num lock*/
        0,	/* Scroll Lock */
        0,	/* Home key */
        0,	/* Up Arrow */
        0,	/* Page Up */
      '-',
        0,	/* Left Arrow */
        0,
        0,	/* Right Arrow */
      '+',
        0,	/* 79 - End key*/
        0,	/* Down Arrow */
        0,	/* Page Down */
        0,	/* Insert Key */
        0,	/* Delete Key */
        0,   0,   0,
        0,	/* F11 Key */
        0,	/* F12 Key */
        0,	/* All other keys are undefined */
    },
    {
        0,  27, '!', '@', '#', '$', '%', '^', '&', '*', /* 9 */
      '(', ')', '_', '+', '\b', /* Backspace */
      '\t',         /* Tab */
      'Q', 'W', 'E', 'R',   /* 19 */
      'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', /* Enter key */
        0,          /* 29   - Control */
      'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', /* 39 */
     '"', '~',   0,        /* Left shift */
     '|', 'Z', 'X', 'C', 'V', 'B', 'N',            /* 49 */
      'M', '<', '>', '?',   0,              /* Right shift */
      '*',
        0,  /* Alt */
      ' ',  /* Space bar */
        0,  /* Caps lock */
        0,  /* 59 - F1 key ... > */
        0,   0,   0,   0,   0,   0,   0,   0,
        0,  /* < ... F10 */
        0,  /* 69 - Num lock*/
        0,  /* Scroll Lock */
        0,  /* Home key */
        0,  /* Up Arrow */
        0,  /* Page Up */
      '-',
        0,  /* Left Arrow */
        0,
        0,  /* Right Arrow */
      '+',
        0,  /* 79 - End key*/
        0,  /* Down Arrow */
        0,  /* Page Down */
        0,  /* Insert Key */
        0,  /* Delete Key */
        0,   0,   0,
        0,  /* F11 Key */
        0,  /* F12 Key */
        0,  /* All other keys are undefined */
    }
};

#endif
