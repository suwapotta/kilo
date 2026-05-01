#include "terminal.h"

#include <complex.h>
#include <stdlib.h>
#include <unistd.h>
#include <termios.h>

static struct termios orig_termios;

void disableRawMode(void)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
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
   */
	raw.c_iflag &= ~(BRKINT | ICRNL | INPCK | ISTRIP | IXON);
	raw.c_oflag &= ~(OPOST);
	raw.c_cflag &= ~(CS8);
	raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}
