#include "output.h"
#include "data.h"

#include <unistd.h>

/* NOTE:
 * VT100 User Guide: http://vt100.net/docs/vt100-ug/chapter3.html
 *  J: Erase in Display
 *  H: Cursor Position (Top left)
 *  C: Cursor Forward
 *  O: Cursor Down
 *  n: Device Status Report
 */

extern struct editorConfig E;

void editorDrawRows(void)
{
	for (int row = 0; row < E.screenRows; row++) {
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
