#ifndef OUTPUT
#define OUTPUT

#include "appendBuffer.h"

void editorDrawRowContent(struct abuf *ab, int row);
void editorDrawRows(struct abuf *ab);
void editorRefreshScreen(void);

#define KILO_VERSION "0.0.1"

#endif // !OUTPUT
