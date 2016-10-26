#include <timer.h>
#include <video.h>

#define TICKS_PER_SECOND 18
#define MAX_TASKS 5

extern void call_task_handler(unsigned long handler);
extern void _hlt(void);

typedef struct {
	unsigned int seconds;
	unsigned long handler;
	bool forever;
} Task;

static unsigned long ticks = 0;
static Task tasks[MAX_TASKS];
static unsigned int tasks_number = 0;

static void remove(int index);

/* Called 18 times a second */
void timer_handler(void) {
	int i;

	ticks++;
	for (i = 0; i < tasks_number; i++) {
		Task task = tasks[i];
		if (ticks % (TICKS_PER_SECOND * task.seconds) == 0) {
			call_task_handler(task.handler);
			if (!task.forever) {
				remove(i--);
			}
		}
	}

}

/*
*	Adds a task to the timer, handler is a function that will be called when "seconds" has passed
*/
bool add_task(unsigned int seconds, unsigned long handler, bool forever) {
	if (contains(handler)) {
		return false;
	}

	if (tasks_number == MAX_TASKS - 1) {
		return false;
	}

	tasks[tasks_number].seconds = seconds;
	tasks[tasks_number].handler = handler;
	tasks[tasks_number].forever = forever;
	tasks_number++;

	return true;
}

static void remove(int index) {
	int i;
	for (i = index; i < tasks_number - 1; i++) {
		tasks[i] = tasks[i + 1];
	}
	tasks_number--;
}

bool contains(unsigned long handler) {
	int i;
	for (i = 0; i < tasks_number; i++) {
		if (tasks[i].handler == handler) {
			return true;
		}
	}
	return false;
}

bool remove_task(unsigned long handler) {
	int i;
	for (i = 0; i < tasks_number; i++) {
		if (tasks[i].handler == handler) {
			remove(i);
			return true;
		}
	}
	return false;
}

void wait(unsigned long millis) {
	unsigned long waiting_ticks = (millis / 1000) * TICKS_PER_SECOND;
	ticks = 0;

	while (waiting_ticks > ticks) {
		_hlt();
	}
}