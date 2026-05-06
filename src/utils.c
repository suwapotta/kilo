#include "utils.h"
#include "terminal.h"
#include "data.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

extern struct editorConfig E;

void die(const char *s)
{
	write(STDOUT_FILENO, "\x1b[2J", 4);
	write(STDOUT_FILENO, "\x1b[H", 3);

	perror(s);
	exit(1);
}

void initEditor(void)
{
	E.cx = 0;
	E.cy = 0;

	if (getWindowSize(&E.screenRows, &E.screenCols) == -1) {
		die("getWindowSize");
	}
}
