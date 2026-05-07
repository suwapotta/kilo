#include "fileIO.h"
#include "data.h"
#include "utils.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// #define _DEFAULT_SOURCE
#define _GNU_SOURCE
#define _BSD_SOURCE

extern struct editorConfig E;

void editorOpen(char *fileName)
{
	FILE *fp = fopen(fileName, "r");
	if (!fp) {
		die("fopen");
	}

	char *line = NULL;
	size_t lineCap = 0;
	ssize_t lineLen = getline(&line, &lineCap, fp);
	if (lineLen != -1) {
		while (lineLen > 0 && (line[lineLen - 1] == '\n' ||
				       line[lineLen - 1] == '\r')) {
			lineLen--;
		}

		E.row.size = lineLen;
		E.row.chars = malloc(lineLen + 1);
		memcpy(E.row.chars, line, lineLen);
		E.row.chars[lineLen] = '\0';
		E.numrows = 1;
	}

	free(line);
	fclose(fp);
}
