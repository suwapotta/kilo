#include "terminal.h"

#include <stdlib.h>
#include <ctype.h>
#include <stdio.h>
#include <unistd.h>

#define EXIT_CHAR 'Q'

int main(void)
{
	enableRawMode();

	char c;
	while (read(STDIN_FILENO, &c, sizeof(char)) == 1) {
		if (c == EXIT_CHAR) {
			return EXIT_SUCCESS;
		}

		if (iscntrl(c)) {
			printf("[ cntrl ]: %d\r\n", c);
			continue;
		}

		printf("[ ascii ]: %d ('%c')\r\n", c, c);
	}
}
