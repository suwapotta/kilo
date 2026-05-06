#include "input.h"
#include "terminal.h"
#include "data.h"

#include <stdlib.h>
#include <unistd.h>

extern struct editorConfig E;

void editorMoveCursor(char key)
{
	switch (key) {
	case 'h':
		E.cx--;
		break;
	case 'j':
		E.cy++;
		break;
	case 'k':
		E.cy--;
		break;
	case 'l':
		E.cx++;
		break;
	}
}

void editorProcessKeypress(void)
{
	char c = editorReadKey();

	switch (c) {
	case CTRL_KEY('q'):
		write(STDOUT_FILENO, "\x1b[2J", 4);
		write(STDOUT_FILENO, "\x1b[H", 3);

		exit(0);
		break;

	case 'h':
	case 'j':
	case 'k':
	case 'l':
		editorMoveCursor(c);
		break;
	}
}
