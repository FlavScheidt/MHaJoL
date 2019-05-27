#include "../../lib/join.h"
int main(int argc, char ** argv)
{
	int nResult;
	clock_t init, end;
	int sel =0;
	char selectivity[4];
	//char fileName[50];
	//char fileName2[50];
	//char aux[50];

	//aux[0] = '\0';
	//Arguments
	if (argc > 1)
	{
		strcpy(selectivity,argv[1]);
		sel=1;
	}
	else
		strcpy(selectivity, "no");

	tamCustomer = TAM_CUSTOMER;
	readCustomerColumn("/home/fscristo/MHaJoL/tbl/customer.tbl", c_customer);

	tamOrders = TAM_ORDERS;
	if (strcmp(selectivity, "10") == 0)
			readOrdersColumn("/home/fscristo/MHaJoL/tbl/orders_10.tbl", c_orders, sel);
	else if (strcmp(selectivity, "20") == 0)
			readOrdersColumn("/home/fscristo/MHaJoL/tbl/orders_20.tbl", c_orders, sel);
	else if (strcmp(selectivity, "30") == 0)
			readOrdersColumn("/home/fscristo/MHaJoL/tbl/orders_30.tbl", c_orders, sel);
	else if (strcmp(selectivity, "40") == 0)
			readOrdersColumn("/home/fscristo/MHaJoL/tbl/orders_40.tbl", c_orders, sel);
	else if (strcmp(selectivity, "50") == 0)
			readOrdersColumn("/home/fscristo/MHaJoL/tbl/orders_50.tbl", c_orders, sel);
	else if (strcmp(selectivity, "60") == 0)
			readOrdersColumn("/home/fscristo/MHaJoL/tbl/orders_60.tbl", c_orders, sel);
	else if (strcmp(selectivity, "70") == 0)
			readOrdersColumn("/home/fscristo/MHaJoL/tbl/orders_70.tbl", c_orders, sel);
	else if (strcmp(selectivity, "80") == 0)
			readOrdersColumn("/home/fscristo/MHaJoL/tbl/orders_80.tbl", c_orders, sel);
	else if (strcmp(selectivity, "90") == 0)
			readOrdersColumn("/home/fscristo/MHaJoL/tbl/orders_90.tbl", c_orders, sel);
	else
			readOrdersColumn("/home/fscristo/MHaJoL/tbl/orders.tbl", c_orders, sel);


	printf("Vectorized Cuckoo Join \n");
	printf("-----------------\n");
	printf("selectivity %s\n", selectivity);

	init = clock();
	likwid_markerInit();
	nResult=vivid256Join(c_customer, c_orders);
	likwid_markerClose();
	end = clock();
	printf("%d linhas\n", nResult);
	printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	return 1;
}
