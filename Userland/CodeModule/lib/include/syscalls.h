#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

#define STDIN	0
#define STDOUT	1
#define STDERR	2
#define STDDATA 3

typedef enum {
	false = 0,
	true = 1,
} bool;

enum {
	SYS_EXIT,
	SYS_CLEAR,
	SYS_READ,
	SYS_WRITE,
	SYS_VIDEO,
	SYS_DRAW,
	SYS_SBRK,
	SYS_MEMBLOCK,
	SYS_FREEBLOCK,
	
	SYS_TIME,
	SYS_DATE,
	SYS_SET_TIME,
	SYS_SET_DATE,
	SYS_WAIT,
};

/* Termina la ejecucion del OS */
int exit(int code);
/* Borra la pantalla de la consola */
void clear(void);
/* Retorna la cantidad de caracteres leidos */
int read(unsigned int fd, const void * buffer, unsigned int size);
/* Retorna la cantidad de caracteres escritos */
int write(unsigned int fd, const void * buffer, unsigned int size);
/* Entra en modo video con los parametros dados */
int video(unsigned int width, unsigned int height, unsigned int bpp);
/* Pinta el pixel x,y con el color pasado en formato RGB */
int draw(unsigned int x, unsigned int y, uint8_t r, uint8_t g, uint8_t b);
/* Aumenta el tamaño del segmento de datos y devuelve el nuevo tope. Si increment es 0, devuelve el tope anterior */
void * sbrk(unsigned int increment);
/* Devuelve un puntero al comienzo de un bloque de memora alocado para un proceso */
void * memblock(unsigned int size);
/* Libera la memoria reservada por un proceso */
int freeblock(void * block_start);
/* Habilita/Deshabilita el reloj en la esquina superior derecha de la consola */
void time(void);
/* Retorna la fecha como un string con formato dd/mm/yyyy */
char * date(void);
/* Cambia la hora con los parametros dados */
int set_time(unsigned int hour, unsigned int minutes, unsigned int seconds);
/* Cambia la fecha con los parametros dados */
int set_date(unsigned int day, unsigned int month, unsigned int year);
/* Espera por una cantidad de milisegundos */
int wait(unsigned long millis);

#endif
