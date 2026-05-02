#include "output.h"

#include <stddef.h>
#include <unistd.h>

/*
 * VT100 User Guide: http://vt100.net/docs/vt100-ug/chapter3.html
 *  J: Erase in Display
 *  H: Cursor Position (Top left)
 */

void editorDrawRows(void)
{
	for (size_t column = 0; column < 24; column++) {
		write(STDOUT_FILENO, "~\r\n", 3);
	}
}

void editorRefreshScreen(void)
{
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);

	editorDrawRows();

	write(STDOUT_FILENO, "\x1b[H", 3);
}
