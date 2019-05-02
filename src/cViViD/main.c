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
//
//	if (strcmp(selectivity, "no") == 0)
//	{
//		strcpy(fileName, "/home/fscristo/MHaJoL/tbl/order.tbl");
	//	strcpy(aux, "tbl/orders.tbl");
	//	aux[14]='\0';
	//	strcat(fileName, aux);
//	}
//	else
//	{
//		strcpy(fileName, PATH_MAIN);
//		strcpy(aux, "tbl/orders_");
//		strcat(fileName, "aux");
//		strcat(fileName, selectivity);
//		aux[0]='\0';
//		strcpy(aux, ".tbl");
//		strcat(fileName, aux);
//	}

	//aux[0]='\0';
	//strcpy(fileName2, "/home/fscristo/MHaJoL/tbl/customer.tbl");
	//strcpy(aux, "tbl/customer.tbl");
	//strcat(fileName2, aux);
	// tamCustomer = countLines(fileName2);
	tamCustomer = TAM_CUSTOMER;
	// c_customer = malloc(tamCustomer*sizeof(column_customer));
	readCustomerColumn("/home/fscristo/MHaJoL/tbl/customer.tbl", c_customer);

	// tamOrders = countLines(fileName);
	tamOrders = TAM_ORDERS;
	// c_orders = malloc(tamOrders*sizeof(column_orders));
	readOrdersColumn("/home/fscristo/MHaJoL/tbl/orders.tbl", c_orders, sel);

	printf("Vectorized Cuckoo Join \n");
	printf("-----------------\n");
	printf("selectivity %s\n", selectivity);

	init = clock();

	likwid_markerInit();
	nResult=cViViDJoin(c_customer, c_orders);
	likwid_markerClose();

	end = clock();
	printf("%d linhas\n", nResult);
	printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	// free(c_orders);
	// free(c_customer);

	pthread_exit(NULL);
	return 1;
}
