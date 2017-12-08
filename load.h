#include <stdio.h>
#include <string.h>
#include <stdlib.h>

/******** VETOR DE STRUCTS CUSTOMER *********/
typedef struct customer
{
	int C_CUSTKEY;
	char C_NAME[25];
	char C_ADDRESS[40];
	long int C_NATIONKEY;
	char C_PHONE[15];
	float C_ACCTBAL;
	char C_MKTSEGMENT[10];
	char C_COMMENT[117];
} customer;

customer *t_customer;

/******** VETOR DE STRUCTS ORDERS ***********/
typedef struct orders
{
	int O_ORDERKEY;
	long int O_CUSTKEY;
	char O_ORDERSTATUS;
	float O_TOTALPRICE;
	int O_ORDERDATE; //date
	char O_ORDERPRIORITY[15];
	char O_CLERK[15];
	int O_SHIPPRIORITY;
	char O_COMMENT[79];
} orders;

orders *t_orders;

/******** FUNÇÕES ***********/
void printOrders(orders *t_orders, int tam);
void readOrders(char fileName[10], FILE *file, orders *t_orders);
void printCustomer(customer *t_customer, int tam);
void readCustomer(char fileName[10], FILE *file, customer *t_customer);
int findSeparator(int init, size_t len, char *line);
int countLines(char fileName[10]);
float toFloat(const char* s);
int toInt(char a[]);
