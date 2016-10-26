#ifndef IO_H_
#define IO_H_

char read_port(unsigned short port);

void write_port(unsigned short port, unsigned char data);

char read_port_word(unsigned short port);

void write_port_word(unsigned short port, unsigned short data);

#endif

