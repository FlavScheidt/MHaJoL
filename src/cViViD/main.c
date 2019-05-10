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
	// c_customer = malloc(tamCustomer*sizeof(column_customer));

	switch selectivity {
		case "10": 
			readOrdersColumn("/home/fscristo/MHaJoL/tbl/orders_10.tbl", c_orders, sel);
			break;
		case "20": 
			readOrdersColumn("/home/fscristo/MHaJoL/tbl/orders_20.tbl", c_orders, sel);
			break;
		case "30": 
			readOrdersColumn("/home/fscristo/MHaJoL/tbl/orders_30.tbl", c_orders, sel);
			break;
		case "40": 
			readOrdersColumn("/home/fscristo/MHaJoL/tbl/orders_40.tbl", c_orders, sel);
			break;
		case "50": 
			readOrdersColumn("/home/fscristo/MHaJoL/tbl/orders_50.tbl", c_orders, sel);
			break;
		case "60": 
			readOrdersColumn("/home/fscristo/MHaJoL/tbl/orders_60.tbl", c_orders, sel);
			break;
		case "70": 
			readOrdersColumn("/home/fscristo/MHaJoL/tbl/orders_70.tbl", c_orders, sel);
			break;
		case "80": 
			readOrdersColumn("/home/fscristo/MHaJoL/tbl/orders_80.tbl", c_orders, sel);
			break;
		case "90": 
			readOrdersColumn("/home/fscristo/MHaJoL/tbl/orders_90.tbl", c_orders, sel);
			break;

		default:
			readOrdersColumn("/home/fscristo/MHaJoL/tbl/orders.tbl", c_orders, sel);
	}

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
