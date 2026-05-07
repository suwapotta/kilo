#ifndef DEFINES
#define DEFINES

#define CTRL_KEY(key) ((key) & 0x1f)

#define KILO_VERSION "0.0.1"

enum editorKey {
	ARROW_LEFT = 1000,
	ARROW_DOWN,
	ARROW_UP,
	ARROW_RIGHT,
	DEL_KEY,
	HOME_KEY,
	END_KEY,
	PAGE_UP,
	PAGE_DOWN
};

#endif // !DEFINES
