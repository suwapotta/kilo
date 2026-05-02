#include "input.h"
#include "terminal.h"

#include <stdlib.h>
#include <unistd.h>

void editorProcessKeypress(void)
{
	char c = editorReadKey();

	switch (c) {
	case CTRL_KEY('q'):
		write(STDOUT_FILENO, "\x1b[2J", 4);
		write(STDOUT_FILENO, "\x1b[H", 3);

		exit(0);
		break;
	}
}
