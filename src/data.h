#include <termios.h>

#ifndef DATA
#define DATA

typedef struct erow {
	int size;
	char *chars;
} erow;

struct editorConfig {
	int cx;
	int cy;
	int screenRows;
	int screenCols;
	int numrows;
	erow row;
	struct termios orig_termios;
};

#endif // !DATA
