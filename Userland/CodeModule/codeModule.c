#include <shell.h>

int main() {

	init_shell();

	while (1) {
		update_shell();
	}

	return 0;
}