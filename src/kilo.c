#include "terminal.h"
#include "utils.h"

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

#define CTRL_KEY(key) ((key) & 0x1f)

int main(void)
{
	enableRawMode();

	while (true) {
		char c = '\0';
		if (read(STDIN_FILENO, &c, sizeof(char)) == -1 &&
		    errno != EAGAIN) {
			die("read");
		}

		if (iscntrl(c)) {
			printf("[ cntrl ]: %d\r\n", c);
		} else {
			printf("[ ascii ]: %d (\'%c\')\r\n", c, c);
		}

		if (c == CTRL_KEY('q')) {
			break;
		}
	}
	return EXIT_SUCCESS;
}
