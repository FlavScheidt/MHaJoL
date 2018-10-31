#include "../../../join.h"

int main(int argc, char ** argv)
{
	int tamCustomer, tamOrders;
	float *t_result;
	int nResult;
	clock_t init, end;
	int sel =0;
	char selectivity[4];
	char fileName[50];

	int tamResult = 150000;

	//nBuckets = 4194304;

	//Arguments
	if (argc > 1)
	{
		strcpy(selectivity,argv[1]);
		sel=1;
	}
	else
		strcpy(selectivity, "no\0");

	if (strcmp(selectivity, "no") == 0)
		strcpy(fileName, "/home/flav/Mestrado/MHaJoL/tbl/orders.tbl\0");
	else
	{
		strcpy(fileName, "/home/flav/Mestrado/MHaJoL/tbl/orders_");
		strcat(fileName, selectivity);
		strcat(fileName, ".tbl\0");
	}

	t_result = malloc(tamResult*sizeof(float));

	tamCustomer = countLines("/home/flav/Mestrado/MHaJoL/tbl/customer.tbl");
	c_customer = malloc(tamCustomer*sizeof(column_customer));
	readCustomerColumn("/home/flav/Mestrado/MHaJoL/tbl/customer.tbl", c_customer);

	tamOrders = countLines(fileName);
	c_orders = malloc(tamOrders*sizeof(column_orders));
	readOrdersColumn(fileName, c_orders, sel);

	//Result Table
	for (int i=0; i<tamResult; i++)
		t_result[i] = 0.0;

	printf("Cuckoo Join\n");
	printf("-----------------\n");
	printf("selectivity %s\n", selectivity);
	init = clock();
	likwid_markerInit();
	nResult=cuckooHash(c_customer, c_orders, tamCustomer, tamOrders, t_result, CUCKOO_SIZE);
	likwid_markerClose();
	end = clock();
	printf("%d linhas\n", nResult);
	printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	free(c_orders);
	free(c_customer);
	
	return 1;
}