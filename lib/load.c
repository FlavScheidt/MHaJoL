#include "load.h"

inline void printCustomerTuple(tuples_customer *t_customer, int tam)
{
	for (int i=0; i<tam; i++)
		printf("%d; %s; %s; %ld; %s; %f; %s; %s \n", t_customer[i].C_CUSTKEY, t_customer[i].C_NAME, t_customer[i].C_ADDRESS, t_customer[i].C_NATIONKEY, t_customer[i].C_PHONE, t_customer[i].C_ACCTBAL, t_customer[i].C_MKTSEGMENT, t_customer[i].C_COMMENT);
}

inline void readCustomerTuple(char fileName[50], tuples_customer *t_customer)
{
	ssize_t read;
	size_t len = 0;
	char *line = NULL;
	int lineInit = 0;
	int lineEnd = 0;
	char dest[100];
	int nLine = 0;
	FILE *file;

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
	free(line);
	fclose(file);
}

inline void printOrdersTuple(tuples_orders *t_orders, int tam)
{
	for (int i=0; i<tam; i++)
		printf("%d; %d; %c; %f; %d; %s; %s; %d, %s \n", t_orders[i].O_ORDERKEY, t_orders[i].O_CUSTKEY, t_orders[i].O_ORDERSTATUS, t_orders[i].O_TOTALPRICE, t_orders[i].O_ORDERDATE, t_orders[i].O_ORDERPRIORITY, t_orders[i].O_CLERK, t_orders[i].O_SHIPPRIORITY, t_orders[i].O_COMMENT);
}

inline void readOrdersTuple(char fileName[50], tuples_orders *t_orders)
{
	char read;
	size_t len = 0;
	char *line = NULL;
	int lineInit = 0;
	int lineEnd = 0;
	char dest[100];
	int nLine = 0;
	FILE *file;

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

inline void readOrdersColumn(char fileName[50], column_orders c_orders[TAM_ORDERS], int type)
{
	ssize_t read;
	size_t len = 0;
	char *line = NULL;
	int lineInit = 0;
	int lineEnd = 0;
	char dest[100];
	int nLine = 0;
	FILE *file;

	file = fopen(fileName, "r+");
	rewind(file);

	if (type == 0)
	{
		while ((read = getline(&line, &len, file)) != -1)
		{
			lineInit = 0;

			//int O_ORDERKEY;
			lineEnd = findSeparator(0, len, line);
			lineInit = lineEnd+1;

			//long int O_CUSTKEY;
			lineEnd = findSeparator(lineInit, len, line);
			strncpy(dest, &line[lineInit], lineEnd-lineInit);
			dest[lineEnd-lineInit] = '\0';
			lineInit = lineEnd+1;

			c_orders[nLine].O_CUSTKEY=toInt(dest);

			//char O_ORDERSTATUS;
			lineEnd = findSeparator(lineInit, len, line);
			lineInit = lineEnd+1;

			//float O_TOTALPRICE;
			lineEnd = findSeparator(lineInit, len, line);
			lineInit = lineEnd+1;

			//int O_ORDERDATE;
			lineEnd = findSeparator(lineInit, len, line);
			lineInit = lineEnd+1;

			//char O_ORDERPRIORITY[15];
			lineEnd = findSeparator(lineInit, len, line);
			lineInit = lineEnd+1;

			//char O_CLERK[15];
			lineEnd = findSeparator(lineInit, len, line);
			lineInit = lineEnd+1;	

			//int O_SHIPPRIORITY;
			lineEnd = findSeparator(lineInit, len, line);;
			lineInit = lineEnd+1;
			
			//char 	O_COMMENT	
			lineEnd = findSeparator(lineInit, len, line);
			lineInit = lineEnd+1;

			nLine++;
		}
	}
	else
	{
		while ((read = getline(&line, &len, file)) != -1)
		{
			lineInit = 0;

			//long int O_CUSTKEY;
			lineEnd = findSeparator(lineInit, len, line);
			strncpy(dest, &line[lineInit], lineEnd-lineInit);
			dest[lineEnd-lineInit] = '\0';
			lineInit = lineEnd+1;

			c_orders[nLine].O_CUSTKEY=toInt(dest);

			nLine++;
		}
	}
	free(line);
	fclose(file);
}

inline void readCustomerColumn(char fileName[50], column_customer c_customer[TAM_CUSTOMER])
{
	ssize_t read;
	size_t len = 0;
	char *line = NULL;
	int lineInit = 0;
	int lineEnd = 0;
	char dest[100];
	int nLine = 0;
	FILE *file;

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
		c_customer[nLine].C_CUSTKEY=toInt(dest);

		//Find C_NAME
		lineEnd = findSeparator(lineInit, len, line);
		lineInit = lineEnd+1;


		//C_ADDRESS
		lineEnd = findSeparator(lineInit, len, line);
		lineInit = lineEnd+1;

		//C_NATIONKEY
		lineEnd = findSeparator(lineInit, len, line);;
		lineInit = lineEnd+1;

		//C_PHONE
		lineEnd = findSeparator(lineInit, len, line);;
		lineInit = lineEnd+1;

		//C_ACCTBAL
		lineEnd = findSeparator(lineInit, len, line);
		strncpy(dest, &line[lineInit], lineEnd-lineInit);
		dest[lineEnd-lineInit] = '\0';
		lineInit = lineEnd+1;

		c_customer[nLine].C_ACCTBAL=toFloat(dest);

		//C_MKTSEGMENT
		lineEnd = findSeparator(lineInit, len, line);
		lineInit = lineEnd+1;	

		//C_COMMENT	
		lineEnd = findSeparator(lineInit, len, line);
		lineInit = lineEnd+1;

		nLine++;
	}
	free(line);
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