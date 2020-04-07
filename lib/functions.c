#include "functions.h"

inline int toInt(char a[]) 
{
	int c, sign, offset, n;
 
 	sign = 0;
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

inline float toFloat(const char* s)
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

inline int countLines(char fileName[50])
{
	ssize_t read = 0;
	size_t len = 0;
	char *line = NULL;	
	int nLines=0;
	FILE *file = fopen(fileName, "r+");

	while ((read = getline(&line, &len, file)) != -1)
	{
		nLines++;
	}
	free(line);
	fclose(file);

	return nLines;
}

inline int findSeparator(int init, size_t len, char *line)
{
	for (size_t i=init; i<len; i++)
	{
		//printf("%c", line[i]);
		if (line[i] == '|')
			return i;
	}
	return -1;
}


uint32_t popCounter(int n)
{
	uint32_t i = (uint32_t)n;
	return (((i<<31)>>31)+((i<<30)>>31)+((i<<29)>>31)+((i<<28)>>31)+((i<<27)>>31)+((i<<26)>>31)+((i<<25)>>31)+((i<<24)>>31)+((i<<23)>>31)+((i<<22)>>31)+((i<<21)>>31)+((i<<20)>>31)+((i<<19)>>31)+((i<<18)>>31)+((i<<17)>>31)+((i<<16)>>31));
}

// inline void handle_error (int retval)
// {
//      printf("PAPI error %d: %s\n", retval, PAPI_strerror(retval));
//      exit(1);
// }
