#include "functions.h"

#define TAM_CUSTOMER 150000
#define TAM_ORDERS 1500000

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
} tuples_customer;

tuples_customer t_customer[TAM_CUSTOMER];

/******** VETOR DE STRUCTS ORDERS ***********/
typedef struct orders
{
	int O_ORDERKEY;
	int O_CUSTKEY;
	char O_ORDERSTATUS;
	float O_TOTALPRICE;
	int O_ORDERDATE; //date
	char O_ORDERPRIORITY[15];
	char O_CLERK[15];
	int O_SHIPPRIORITY;
	char O_COMMENT[79];
} tuples_orders;

tuples_orders t_orders[TAM_ORDERS];

/******** VETOR DE STRUCTS COLUMN CUSTOMER *********/
typedef struct customer_c
{
	unsigned int C_CUSTKEY;
	float C_ACCTBAL;
} column_customer;

column_customer c_customer[TAM_CUSTOMER];

/******** VETOR DE STRUCTS COLUMN ORDERS ***********/
typedef struct orders_c
{
	unsigned int O_CUSTKEY;
} column_orders;

column_orders c_orders[TAM_ORDERS];


/******** FUNÇÕES ***********/
void printOrdersTuple(tuples_orders t_orders[TAM_ORDERS], int tam);
void readOrdersTuple(char fileName[50], tuples_orders t_orders[TAM_ORDERS]);
void printCustomerTuple(tuples_customer *t_customer, int tam);
void readCustomerTuple(char fileName[50], tuples_customer *t_customer);

void readOrdersColumn(char fileName[50], column_orders c_orders[TAM_ORDERS], int type);
void readCustomerColumn(char fileName[50], column_customer *c_customer);

void load(int sel, float * t_result, column_customer **c_customer, column_orders *c_orders[TAM_ORDERS], int * tamOrders, int * tamCustomer, int tamResult, char fileName[50]);
