#include "bloomFilter.h"

int main(int argc, char ** argv)
{
	int tamCustomer, tamOrders;
	float *t_result;
	int nResult;
	clock_t init, end;
	int nBuckets;
	int nHash;
	int param=0;
	int sel =0;
	char selectivity[4];
	char fileName[50];

	//Arguments
	if (argc > 1)
	{
		strcpy(selectivity,argv[1]);
		sel=1;
		if (argc > 2)
		{
			param = 1;
			 nBuckets = toInt(argv[2]);
			 if (argc > 3)
			 	nHash = toInt(argv[3]);
			 else 
			 	nHash = 4;
		}
		else
			nBuckets = 4194304;
	}
	else
		strcpy(selectivity, "no\0");

	if (strcmp(selectivity, "no") == 0)
		strcpy(fileName, "/home/flav/mestrado/BloomJoin/tbl/orders.tbl\0");
	else
	{
		strcpy(fileName, "/home/flav/mestrado/BloomJoin/tbl/orders_");
		strcat(fileName, selectivity);
		strcat(fileName, ".tbl\0");
	}

	// Load Tables
	tamCustomer = countLines("/home/flav/mestrado/BloomJoin/tbl/customer.tbl");
	c_customer = malloc(tamCustomer*sizeof(column_customer));
	readCustomerColumn("/home/flav/mestrado/BloomJoin/tbl/customer.tbl", c_customer);

	tamOrders = countLines(fileName);
	c_orders = malloc(tamOrders*sizeof(column_orders));
	readOrdersColumn(fileName, c_orders, sel);

	//Result Table
	t_result = malloc(tamOrders*sizeof(float));
	for (int i=0; i<tamOrders; i++)
		t_result[i] = 0.0;

	if (param==0)
	{
		//Nested Loop Join
		init = clock();
		nResult=nestedLoopJoin(c_customer, c_orders, tamCustomer, tamOrders, t_result);
		end = clock();

		printf("Nested Loop Join\n");
		printf("-----------------\n");
		printf("%d linhas\n", nResult);
		printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

		//Hash Loop Join
		printf("Hash Join\n");
		printf("-----------------\n");
		init = clock();
		nResult=hashJoin(c_customer, c_orders, tamCustomer, tamOrders, t_result);
		end = clock();
		printf("%d linhas\n", nResult);
		printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

		printf("Bloom  Join\n");
		printf("-----------------\n");
		init = clock();
		nResult=bloomFilter(c_customer, c_orders, tamCustomer, tamOrders, t_result, nBuckets);
		end = clock();
		printf("%d linhas\n", nResult);
		printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

		printf("Bloom Nested Join\n");
		printf("-----------------\n");
		init = clock();
		nResult=bloomNested(c_customer, c_orders, tamCustomer, tamOrders, t_result, nBuckets);
		end = clock();
		printf("%d linhas\n", nResult);
		printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

		printf("Bloom Hash Join\n");
		printf("-----------------\n");
		init = clock();
		nResult=bloomHash(c_customer, c_orders, tamCustomer, tamOrders, t_result, nBuckets);
		end = clock();
		printf("%d linhas\n", nResult);
		printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
	}
	else
	{
		printf("Bloom Join\n");
		printf("-----------------\n");
		printf("%d Buckets %d Hash Functions\n", nBuckets, nHash);
		init = clock();
		nResult=bloomFilterParam(c_customer, c_orders, tamCustomer, tamOrders, t_result, nBuckets, nHash);
		end = clock();
		printf("%d linhas\n", nResult);
		printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
	}	
	return 1;
}