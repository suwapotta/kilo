#include <termios.h>

#ifndef DATA
#define DATA

struct editorConfig {
	int cx;
	int cy;
	int screenRows;
	int screenCols;
	struct termios orig_termios;
};

#endif // !DATA
