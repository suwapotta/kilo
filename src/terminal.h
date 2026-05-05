#ifndef TERMINAL_H
#define TERMINAL_H

void disableRawMode(void);
void enableRawMode(void);
char editorReadKey(void);
int getWindowSize(int *rows, int *cols);

#endif // !TERMINAL_H
