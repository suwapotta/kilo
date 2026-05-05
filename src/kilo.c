#include "terminal.h"
#include "input.h"
#include "output.h"
#include "utils.h"

#include <stdbool.h>
#include <stdlib.h>

int main(void)
{
	enableRawMode();
	initEditor();

	while (true) {
		editorRefreshScreen();
		editorProcessKeypress();
	}

	return EXIT_SUCCESS;
}
