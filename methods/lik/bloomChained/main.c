#include "../../join.h"

int main(int argc, char ** argv)
{
	int tamCustomer, tamOrders;
	float *t_result;
	int nResult;
	clock_t init, end;
	int nBuckets;
	int sel = 0;

	char fileName[50];

	int tamResult = 150000;

	//nBuckets = 4194304;
	nBuckets = HASH_BUCKETS;

	strcpy(fileName, "/home/flav/mestrado/MHaJoL/tbl/orders.tbl\0");

	t_result = malloc(tamResult*sizeof(float));

	// Load Tables
	tamCustomer = countLines("/home/flav/mestrado/MHaJoL/tbl/customer.tbl");
	c_customer = malloc(tamCustomer*sizeof(column_customer));
	readCustomerColumn("/home/flav/mestrado/MHaJoL/tbl/customer.tbl", c_customer);

	tamOrders = countLines(fileName);
	c_orders = malloc(tamOrders*sizeof(column_orders));
	readOrdersColumn(fileName, c_orders, sel);

	//Result Table
	for (int i=0; i<tamResult; i++)
		t_result[i] = 0.0;

	printf("Bloom Hash Join\n");
	printf("-----------------\n");
	init = clock();
	nResult=bloomHash(c_customer, c_orders, tamCustomer, tamOrders, t_result, nBuckets);
	end = clock();
	printf("%d linhas\n", nResult);
	printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	free(c_orders);
	free(c_customer);
	
	return 1;
}