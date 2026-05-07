#include "terminal.h"
#include "input.h"
#include "output.h"
#include "utils.h"
#include "fileIO.h"

#include <stdbool.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	enableRawMode();
	initEditor();
	if (argc >= 2) {
		editorOpen(argv[1]);
	}

	while (true) {
		editorRefreshScreen();
		editorProcessKeypress();
	}
	return EXIT_SUCCESS;
}
