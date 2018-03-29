#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <likwid.h>

int findSeparator(int init, size_t len, char *line);
int countLines(char fileName[50]);
float toFloat(const char* s);
int toInt(char a[]);