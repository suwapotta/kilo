#ifndef INPUT
#define INPUT

#define CTRL_KEY(key) ((key) & 0x1f)

void editorMoveCursor(char key);
void editorProcessKeypress(void);

#endif // !INPUT
