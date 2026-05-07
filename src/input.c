#include "input.h"
#include "terminal.h"
#include "data.h"
#include "defines.h"

#include <stdlib.h>
#include <unistd.h>

extern struct editorConfig E;

void editorMoveCursor(int key)
{
	switch (key) {
	case ARROW_LEFT:
		if (E.cx == 0) {
			break;
		}

		E.cx--;
		break;
	case ARROW_DOWN:
		if (E.cy + 1 == E.screenRows) {
			break;
		}

		E.cy++;
		break;
	case ARROW_UP:
		if (E.cy == 0) {
			break;
		}

		E.cy--;
		break;
	case ARROW_RIGHT:
		if (E.cx + 1 == E.screenCols) {
			break;
		}

		E.cx++;
		break;
	}
}

void editorProcessKeypress(void)
{
	int c = editorReadKey();

	switch (c) {
	case CTRL_KEY('q'):
		write(STDOUT_FILENO, "\x1b[2J", 4);
		write(STDOUT_FILENO, "\x1b[H", 3);

		exit(0);
		break;

	case HOME_KEY:
		E.cx = 0;
		break;

	case END_KEY:
		E.cx = E.screenCols - 1;
		break;

	case PAGE_UP:
	case PAGE_DOWN: {
		int times = E.screenRows;
		while (times--) {
			editorMoveCursor(c == PAGE_UP ? ARROW_UP : ARROW_DOWN);
		}
	} break;
	case ARROW_UP:
	case ARROW_DOWN:
	case ARROW_LEFT:
	case ARROW_RIGHT:
		editorMoveCursor(c);
		break;
	}
}
