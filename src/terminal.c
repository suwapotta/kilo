#include "terminal.h"
#include "utils.h"
#include "data.h"

#include <errno.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ioctl.h>

struct editorConfig E;

void disableRawMode(void)
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &E.orig_termios) == -1) {
		die("tcgetattr");
	}
}

void enableRawMode(void)
{
	if (tcgetattr(STDIN_FILENO, &E.orig_termios) == -1) {
		die("tcgetattr");
	}
	atexit(disableRawMode);

	struct termios raw = E.orig_termios;
	/*
   *  1. CONTROLs (bitflag)           2. SIGNALs
   *     ICRNL                           Ctrl-M ('\n')
   *     IXON                            Ctrl-S/Q (flow controls)
   *
   *     OPOST                           '\r\n' (output processing)
   *
   *     ECHO                            Key presses
   *     ICANON                          Canonical mode
   *     ISIG                            Ctrl-C/Z
   *     IEXTEN                          Ctrl-V
   *
   *  3. Others: misc
   */
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag &= ~(CS8);
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	raw.c_cc[VMIN] = 0; // byte
	raw.c_cc[VTIME] = 1; // ms

	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
		die("tcsetattr");
	}
}

char editorReadKey(void)
{
	int nread;
	char c;

	while ((nread = read(STDIN_FILENO, &c, sizeof(char))) != 1) {
		if (nread == -1 && errno != EAGAIN) {
			die("read");
		}
	}

	return c;
}

int getWindowSize(int *rows, int *cols)
{
	struct winsize ws;

	if (true || ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 ||
	    ws.ws_col == 0) {
		if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) {
			return -1;
		}

		editorReadKey();
		return -1;
	}

	*cols = ws.ws_col;
	*rows = ws.ws_row;

	return 0;
}
