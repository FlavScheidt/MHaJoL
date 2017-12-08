#include "load.h"

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

int countLines(char fileName[10])
{
	char read;
	size_t len = 0;
	char *line = NULL;	
	int nLines=0;
	FILE *file = fopen(fileName, "r");

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

void printCustomer(customer *t_customer, int tam)
{
	for (int i=0; i<tam; i++)
		printf("%d; %s; %s; %ld; %s; %f; %s; %s \n", t_customer[i].C_CUSTKEY, t_customer[i].C_NAME, t_customer[i].C_ADDRESS, t_customer[i].C_NATIONKEY, t_customer[i].C_PHONE, t_customer[i].C_ACCTBAL, t_customer[i].C_MKTSEGMENT, t_customer[i].C_COMMENT);
}

void readCustomer(char fileName[10], FILE *file, customer *t_customer)
{
	char read;
	size_t len = 0;
	char *line = NULL;
	int lineInit = 0;
	int lineEnd = 0;
	char dest[100];
	int nLine = 0;

	file = fopen(fileName, "r+");
	rewind(file);

	while ((read = getline(&line, &len, file)) != -1)
	{
		lineInit = 0;

		//Find C_CUSTKEY
		lineEnd = findSeparator(0, len, line);
		strncpy(dest, &line[lineInit], lineEnd-lineInit);
		dest[lineEnd-lineInit] = '\0';
		lineInit = lineEnd+1;

		t_customer[nLine].C_CUSTKEY=toInt(dest);

		//Find C_NAME
		lineEnd = findSeparator(lineInit, len, line);
		strncpy(t_customer[nLine].C_NAME, &line[lineInit], lineEnd-lineInit);
		t_customer[nLine].C_NAME[lineEnd-lineInit] = '\0';
		lineInit = lineEnd+1;


		//C_ADDRESS
		lineEnd = findSeparator(lineInit, len, line);
		strncpy(t_customer[nLine].C_ADDRESS, &line[lineInit], lineEnd-lineInit);
		t_customer[nLine].C_ADDRESS[lineEnd-lineInit] = '\0';
		lineInit = lineEnd+1;

		//C_NATIONKEY
		lineEnd = findSeparator(lineInit, len, line);
		strncpy(dest, &line[lineInit], lineEnd-lineInit);
		dest[lineEnd-lineInit] = '\0';
		lineInit = lineEnd+1;

		t_customer[nLine].C_NATIONKEY=toInt(dest);

		//C_PHONE
		lineEnd = findSeparator(lineInit, len, line);
		strncpy(t_customer[nLine].C_PHONE, &line[lineInit], lineEnd-lineInit);
		t_customer[nLine].C_PHONE[lineEnd-lineInit] = '\0';
		lineInit = lineEnd+1;

		//C_ACCTBAL
		lineEnd = findSeparator(lineInit, len, line);
		strncpy(dest, &line[lineInit], lineEnd-lineInit);
		dest[lineEnd-lineInit] = '\0';
		lineInit = lineEnd+1;

		t_customer[nLine].C_ACCTBAL=toFloat(dest);

		//C_MKTSEGMENT
		lineEnd = findSeparator(lineInit, len, line);
		strncpy(t_customer[nLine].C_MKTSEGMENT, &line[lineInit], lineEnd-lineInit);
		t_customer[nLine].C_MKTSEGMENT[lineEnd-lineInit] = '\0';
		lineInit = lineEnd+1;	

		//C_COMMENT	
		lineEnd = findSeparator(lineInit, len, line);
		strncpy(t_customer[nLine].C_COMMENT, &line[lineInit], lineEnd-lineInit);
		t_customer[nLine].C_COMMENT[lineEnd-lineInit] = '\0';
		lineInit = lineEnd+1;

		nLine++;
	}
	fclose(file);
}

void printOrders(orders *t_orders, int tam)
{
	for (int i=0; i<tam; i++)
		printf("%d; %ld; %c; %f; %d; %s; %s; %d, %s \n", t_orders[i].O_ORDERKEY, t_orders[i].O_CUSTKEY, t_orders[i].O_ORDERSTATUS, t_orders[i].O_TOTALPRICE, t_orders[i].O_ORDERDATE, t_orders[i].O_ORDERPRIORITY, t_orders[i].O_CLERK, t_orders[i].O_SHIPPRIORITY, t_orders[i].O_COMMENT);
}

void readOrders(char fileName[10], FILE *file, orders *t_orders)
{
	char read;
	size_t len = 0;
	char *line = NULL;
	int lineInit = 0;
	int lineEnd = 0;
	char dest[100];
	int nLine = 0;

	file = fopen(fileName, "r+");
	rewind(file);

	while ((read = getline(&line, &len, file)) != -1)
	{
		lineInit = 0;

		//int O_ORDERKEY;
		lineEnd = findSeparator(0, len, line);
		strncpy(dest, &line[lineInit], lineEnd-lineInit);
		dest[lineEnd-lineInit] = '\0';
		lineInit = lineEnd+1;

		t_orders[nLine].O_ORDERKEY=toInt(dest);

		//long int O_CUSTKEY;
		lineEnd = findSeparator(lineInit, len, line);
		strncpy(dest, &line[lineInit], lineEnd-lineInit);
		dest[lineEnd-lineInit] = '\0';
		lineInit = lineEnd+1;

		t_orders[nLine].O_CUSTKEY=toInt(dest);

		//char O_ORDERSTATUS;
		lineEnd = findSeparator(lineInit, len, line);
		t_orders[nLine].O_ORDERSTATUS = line[lineInit];
		lineInit = lineEnd+1;

		//float O_TOTALPRICE;
		lineEnd = findSeparator(lineInit, len, line);
		strncpy(dest, &line[lineInit], lineEnd-lineInit);
		dest[lineEnd-lineInit] = '\0';
		lineInit = lineEnd+1;

		t_orders[nLine].O_TOTALPRICE=toFloat(dest);

		//int O_ORDERDATE;
		lineEnd = findSeparator(lineInit, len, line);
		strncpy(dest, &line[lineInit], lineEnd-lineInit);
		dest[lineEnd-lineInit] = '\0';
		lineInit = lineEnd+1;

		t_orders[nLine].O_ORDERDATE=toInt(dest);

		//char O_ORDERPRIORITY[15];
		lineEnd = findSeparator(lineInit, len, line);
		strncpy(t_orders[nLine].O_ORDERPRIORITY, &line[lineInit], lineEnd-lineInit);
		t_orders[nLine].O_ORDERPRIORITY[lineEnd-lineInit] = '\0';
		lineInit = lineEnd+1;

		//char O_CLERK[15];
		lineEnd = findSeparator(lineInit, len, line);
		strncpy(t_orders[nLine].O_CLERK, &line[lineInit], lineEnd-lineInit);
		t_orders[nLine].O_CLERK[lineEnd-lineInit] = '\0';
		lineInit = lineEnd+1;	

		//int O_SHIPPRIORITY;
		lineEnd = findSeparator(lineInit, len, line);
		strncpy(dest, &line[lineInit], lineEnd-lineInit);
		dest[lineEnd-lineInit] = '\0';
		lineInit = lineEnd+1;

		t_orders[nLine].O_SHIPPRIORITY=toInt(dest);
		
		//char 	O_COMMENT	
		lineEnd = findSeparator(lineInit, len, line);
		strncpy(t_orders[nLine].O_COMMENT, &line[lineInit], lineEnd-lineInit);
		t_orders[nLine].O_COMMENT[lineEnd-lineInit] = '\0';
		lineInit = lineEnd+1;

		nLine++;
	}

	fclose(file);
}

// int main()
// {
// 	FILE *file;
// 	int tamCustomer, tamOrders;

// 	tamCustomer = countLines("./csv/customer.csv");
// 	t_customer = malloc(tamCustomer*sizeof(customer));
// 	readCustomer("./csv/customer.csv", file, t_customer);


// 	tamOrders = countLines("./csv/orders.csv");
// 	t_orders = malloc(tamOrders*sizeof(orders));
// 	readOrders("./csv/orders.csv", file, t_orders);
// }