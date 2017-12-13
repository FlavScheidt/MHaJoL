#include "bloomFilter.h"

int main(int argc, char ** argv)
{
	int tamCustomer, tamOrders;
	float *t_result;
	int nResult;
	clock_t init, end;
	int nBuckets;

	//Arguments
	if (argc > 1)
		nBuckets = toInt(argv[1]);
	else
		nBuckets = 4194304;

	// Load Tables
	tamCustomer = countLines("./tbl/customer.tbl");
	c_customer = malloc(tamCustomer*sizeof(tuples_customer));
	readCustomerColumn("./tbl/customer.tbl", c_customer);

	tamOrders = countLines("./tbl/orders.tbl");
	c_orders = malloc(tamOrders*sizeof(tuples_orders));
	readOrdersColumn("./tbl/orders.tbl", c_orders);

	//Result Table
	t_result = malloc(tamOrders*sizeof(float));
	for (int i=0; i<tamOrders; i++)
		t_result[i] = 0.0;

	//Nested Loop Join
	// init = clock();
	// nResult=nestedLoopJoin(c_customer, c_orders, tamCustomer, tamOrders, t_result);
	// end = clock();

	// printf("Nested Loop Join\n");
	// printf("-----------------\n");
	// printf("%d linhas\n", nResult);
	// printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	//Hash Loop Join
	// printf("Hash Join\n");
	// printf("-----------------\n");
	// init = clock();
	// nResult=hashJoin(c_customer, c_orders, tamCustomer, tamOrders, t_result);
	// end = clock();
	// printf("%d linhas\n", nResult);
	// printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	printf("Bloom  Join\n");
	printf("-----------------\n");
	init = clock();
	nResult=bloomFilter(c_customer, c_orders, tamCustomer, tamOrders, t_result, nBuckets);
	end = clock();
	printf("%d linhas\n", nResult);
	printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	// printf("Bloom Nested Join\n");
	// printf("-----------------\n");
	// init = clock();
	// nResult=bloomNested(c_customer, c_orders, tamCustomer, tamOrders, t_result, nBuckets);
	// end = clock();
	// printf("%d linhas\n", nResult);
	// printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	// printf("Bloom Hash Join\n");
	// printf("-----------------\n");
	// init = clock();
	// nResult=bloomHash(c_customer, c_orders, tamCustomer, tamOrders, t_result, nBuckets);
	// end = clock();
	// printf("%d linhas\n", nResult);
	// printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
	
	return 1;
}