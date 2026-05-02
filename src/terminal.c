#include "terminal.h"
#include "utils.h"

#include <complex.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

static struct termios orig_termios;

void disableRawMode(void)
{
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1) {
		die("tcgetattr");
	}
}

void enableRawMode(void)
{
	tcgetattr(STDIN_FILENO, &orig_termios);
	atexit(disableRawMode);

	struct termios raw = orig_termios;
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
