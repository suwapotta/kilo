#include "data.h"
#include "appendBuffer.h"
#include "output.h"
#include "defines.h"

#include <stdio.h>
#include <string.h>
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

void editorDrawRowContent(struct abuf *ab, int row)
{
	if (row < E.numrows) {
		int len = E.row.size;
		if (len > E.screenCols) {
			len = E.screenCols;
		}

		abAppend(ab, E.row.chars, len);
		return;
	}

	if (E.numrows != 0 || row != E.screenRows * 2 / 3) {
		abAppend(ab, "~", 2);
		return;
	}

	char welcome[80];
	int welcomeLen = snprintf(welcome, sizeof(welcome),
				  "Kilo editor -- version %s", KILO_VERSION);

	if (welcomeLen > E.screenCols) {
		welcomeLen = E.screenCols;
	}

	int padding = (E.screenCols - welcomeLen) / 2;
	if (padding) {
		abAppend(ab, "~", 1);
		padding--;
	}

	while (padding--) {
		abAppend(ab, " ", 1);
	}

	abAppend(ab, welcome, welcomeLen);
}

void editorDrawRows(struct abuf *ab)
{
	for (int row = 0; row < E.screenRows; row++) {
		editorDrawRowContent(ab, row);

		abAppend(ab, "\x1b[K", 3);
		if (row < E.screenRows - 1) {
			abAppend(ab, "\r\n", 2);
		}
	}
}

void editorRefreshScreen(void)
{
	struct abuf ab = ABUF_INIT;

	// Clear line
	abAppend(&ab, "\x1b[?25l", 6);
	abAppend(&ab, "\x1b[H", 3);

	editorDrawRows(&ab);

	// Navigation
	char buf[32];
	snprintf(buf, sizeof(buf), "\x1b[%d;%dH", E.cy + 1, E.cx + 1);
	abAppend(&ab, buf, strlen(buf));

	// Cursor
	abAppend(&ab, "\x1b[?25h", 6);
	abAppend(&ab, "\x1b[1 q", 5);

	write(STDOUT_FILENO, ab.b, ab.len);
	abFree(&ab);
}
