#include "functions.h"

int toInt(char a[]) 
{
	int c, sign, offset, n;
 
	if (a[0] == '-') 
		sign = -1;
 
	if (sign == -1)
		offset = 1;
  	else
    	offset = 0;
 
	n = 0;
 
	for (c = offset; a[c] != '\0'; c++)
		n = n * 10 + a[c] - '0';

	if (sign == -1)
	n = -n;

	return n;
}

float toFloat(const char* s)
{
	float rez = 0, fact = 1;
		if (*s == '-')
		{
			s++;
			fact = -1;
  		}

	for (int point_seen = 0; *s; s++)
	{
		if (*s == '.')
		{
			point_seen = 1; 
			continue;
    	}
    	int d = *s - '0';
		if (d >= 0 && d <= 9)
		{
			if (point_seen) fact /= 10.0f;
      			rez = rez * 10.0f + (float)d;
		}
	}
	return rez * fact;
}

int countLines(char fileName[50])
{
	char read;
	size_t len = 0;
	char *line = NULL;	
	int nLines=0;
	FILE *file = fopen(fileName, "r+");

	while ((read = getline(&line, &len, file)) != -1)
		nLines++;

	fclose(file);

	return nLines;
}

int findSeparator(int init, size_t len, char *line)
{
	for (int i=init; i<len; i++)
	{
		//printf("%c", line[i]);
		if (line[i] == '|')
			return i;
	}
	return -1;
}
