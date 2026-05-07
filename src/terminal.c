#include "terminal.h"
#include "utils.h"
#include "data.h"
#include "defines.h"

#include <ctype.h>
#include <errno.h>
#include <stdbool.h>
#include <stdio.h>
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

int editorReadKey(void)
{
	int nread;
	char c;

	while ((nread = read(STDIN_FILENO, &c, sizeof(char))) != 1) {
		if (nread == -1 && errno != EAGAIN) {
			die("read");
		}
	}

	if (c != '\x1b') {
		return c;
	}

	char seq[3];

	if (read(STDIN_FILENO, &seq[0], 1) != 1) {
		return '\x1b';
	}
	if (read(STDIN_FILENO, &seq[1], 1) != 1) {
		return '\x1b';
	}

	if (seq[0] != '[') {
		return '\x1b';
	}

	if (isdigit(seq[1])) {
		if (read(STDIN_FILENO, &seq[2], 1) != 1) {
			return '\x1b';
		}

		if (seq[2] == '~') {
			switch (seq[1]) {
			case '1':
				return HOME_KEY;
			case '3':
				return DEL_KEY;
			case '4':
				return END_KEY;
			case '5':
				return PAGE_UP;
			case '6':
				return PAGE_DOWN;
			case '7':
				return HOME_KEY;
			case '8':
				return END_KEY;
			}
		}
	}

	switch (seq[1]) {
	case 'A':
		return ARROW_UP;
	case 'B':
		return ARROW_DOWN;
	case 'C':
		return ARROW_RIGHT;
	case 'D':
		return ARROW_LEFT;
	case 'H':
		return HOME_KEY;
	case 'F':
		return END_KEY;
	}

	if (seq[0] == 'O') {
		switch (seq[1]) {
		case 'H':
			return HOME_KEY;
		case 'F':
			return END_KEY;
		}
	}

	return c;
}

int getCursorPosition(int *rows, int *cols)
{
	char buf[32];
	unsigned int i = 0;

	if (write(STDOUT_FILENO, "\x1b[6n", 4) != 4) {
		return -1;
	}
	printf("\r\n");

	while (i < sizeof(buf) - 1) {
		if (read(STDIN_FILENO, &buf[i], sizeof(char)) != 1) {
			break;
		}

		if (buf[i] == 'R') {
			break;
		}

		i++;
	}
	buf[i] = '\0';

	if (buf[0] != '\x1b' || buf[1] != '[') {
		return -1;
	}

	if (sscanf(&buf[2], "%d;%d", rows, cols) != 2) {
		return -1;
	}

	return 0;
}

int getWindowSize(int *rows, int *cols)
{
	struct winsize ws;

	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1 || ws.ws_col == 0) {
		if (write(STDOUT_FILENO, "\x1b[999C\x1b[999B", 12) != 12) {
			return -1;
		}
		return getCursorPosition(rows, cols);
	}

	*cols = ws.ws_col;
	*rows = ws.ws_row;

	return 0;
}
