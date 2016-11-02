#ifndef TIMER_H
#define TIMER_H

#include <types.h>

void timer_handler(void);

bool add_task(unsigned int seconds, unsigned long handler, bool forever);

bool remove_task(unsigned long handler);

bool contains(unsigned long handler);

void wait(unsigned long millis);

#endif
