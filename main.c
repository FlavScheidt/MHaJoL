#include "bloomFilter.h"

int main()
{
	FILE *file;
	int tamCustomer, tamOrders;
	float *t_result;
	int nResult;
	time_t init, end;

	// Load Tables
	tamCustomer = countLines("./tbl/customer.tbl");
	t_customer = malloc(tamCustomer*sizeof(customer));
	readCustomer("./tbl/customer.tbl", file, t_customer);

	tamOrders = countLines("./tbl/orders.tbl");
	t_orders = malloc(tamOrders*sizeof(orders));
	readOrders("./tbl/orders.tbl", file, t_orders);

	//Result Table
	t_result = malloc(tamOrders*sizeof(float));
	for (int i=0; i<tamOrders; i++)
		t_result[i] = 0.0;

	//Nested Loop Join
	// init = time(NULL);
	// nResult=nestedLoopJoin(t_customer, t_orders, tamCustomer, tamOrders, t_result);
	// end = time(NULL);

	// printf("Nested Loop Join\n");
	// printf("-----------------\n");
	// printf("%d linhas\n", nResult);
	// printf("%.f segundos \n\n", difftime(end,init));

	//Hash Loop Join
	init = time(NULL);
	nResult=hashJoin(t_customer, t_orders, tamCustomer, tamOrders, t_result);
	end = time(NULL);

	printf("Hash Join\n");
	printf("-----------------\n");
	printf("%d linhas\n", nResult);
	printf("%.f segundos \n\n", difftime(end,init));

	init = time(NULL);
	nResult=bloomFilter(t_customer, t_orders, tamCustomer, tamOrders, t_result);
	end = time(NULL);

	printf("Bloom  Join\n");
	printf("-----------------\n");
	printf("%d linhas\n", nResult);
	printf("%.f segundos \n\n", difftime(end,init));
	
	return 1;
}