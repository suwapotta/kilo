#include "data.h"
#include "appendBuffer.h"

#include <unistd.h>

/* NOTE:
 * VT100 User Guide: http://vt100.net/docs/vt100-ug/chapter3.html
 *  J: Erase in Display
 *  H: Cursor Position (Top left)
 *  C: Cursor Forward
 *  O: Cursor Down
 *  n: Device Status Report
 *  h: Set Mode
 *  l: Reset Mode
 *  K: Erase in line
 */

extern struct editorConfig E;

void editorDrawRows(struct abuf *ab)
{
	for (int row = 0; row < E.screenRows; row++) {
		abAppend(ab, "~", 1);

		abAppend(ab, "\x1b[K", 3);
		if (row < E.screenCols - 1) {
			abAppend(ab, "\r\n", 2);
		}
	}
}

void editorRefreshScreen(void)
{
	struct abuf ab = ABUF_INIT;

	abAppend(&ab, "\x1b[?25l", 6);
	abAppend(&ab, "\x1b[H", 3);

	editorDrawRows(&ab);

	abAppend(&ab, "\x1b[H", 3);
	abAppend(&ab, "\x1b[?25h", 6);

	write(STDOUT_FILENO, ab.b, ab.len);
	abFree(&ab);
}
