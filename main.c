#include "join.h"

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

	int tamResult = 150000;

	//nBuckets = 4194304;
	nBuckets = HASH_BUCKETS;

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
			nBuckets = HASH_BUCKETS;
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

	t_result = malloc(tamResult*sizeof(float));

	if (param==0)
	{
		/************************************************/
		//Nested Loop Join
		/************************************************/
		// // Load Tables
		// tamCustomer = countLines("/home/flav/mestrado/BloomJoin/tbl/customer.tbl");
		// c_customer = malloc(tamCustomer*sizeof(column_customer));
		// readCustomerColumn("/home/flav/mestrado/BloomJoin/tbl/customer.tbl", c_customer);

		// tamOrders = countLines(fileName);
		// c_orders = malloc(tamOrders*sizeof(column_orders));
		// readOrdersColumn(fileName, c_orders, sel);

		// //Result Table
		// for (int i=0; i<tamResult; i++)
		// 	t_result[i] = 0.0;


		// init = clock();
		// nResult=nestedLoopJoin(c_customer, c_orders, tamCustomer, tamOrders, t_result);
		// end = clock();

		// printf("Nested Loop Join\n");
		// printf("-----------------\n");
		// printf("%d linhas\n", nResult);
		// printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

		// free(c_orders);
		// free(c_customer);

		/****************************************/
		//Hash Join
		/****************************************/
		// //Load Tables
		// tamCustomer = countLines("/home/flav/mestrado/BloomJoin/tbl/customer.tbl");
		// c_customer = malloc(tamCustomer*sizeof(column_customer));
		// readCustomerColumn("/home/flav/mestrado/BloomJoin/tbl/customer.tbl", c_customer);

		// tamOrders = countLines(fileName);
		// c_orders = malloc(tamOrders*sizeof(column_orders));
		// readOrdersColumn(fileName, c_orders, sel);

		// //Result Table
		// for (int i=0; i<tamResult; i++)
		// 	t_result[i] = 0.0;

		// printf("Hash Join\n");
		// printf("-----------------\n");
		// init = clock();
		// nResult=hashJoin(c_customer, c_orders, tamCustomer, tamOrders, t_result);
		// end = clock();
		// printf("%d linhas\n", nResult);
		// printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

		// free(c_orders);
		// free(c_customer);

		// /*****************************************/
		// // printf("Bloom  Join\n");
		// /*****************************************/
		// // Load Tables
		// tamCustomer = countLines("/home/flav/mestrado/BloomJoin/tbl/customer.tbl");
		// c_customer = malloc(tamCustomer*sizeof(column_customer));
		// readCustomerColumn("/home/flav/mestrado/BloomJoin/tbl/customer.tbl", c_customer);

		// tamOrders = countLines(fileName);
		// c_orders = malloc(tamOrders*sizeof(column_orders));
		// readOrdersColumn(fileName, c_orders, sel);

		// //Result Table
		// for (int i=0; i<tamResult; i++)
		// 	t_result[i] = 0.0;

		// printf("Bloom Join \n");
		// printf("-----------------\n");
		// init = clock();
		// nResult=bloomFilter(c_customer, c_orders, tamCustomer, tamOrders, t_result, nBuckets);
		// end = clock();
		// printf("%d linhas\n", nResult);
		// printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

		// free(c_orders);
		// free(c_customer);

		// /***************************************/
		// // Bloom Nested Join
		// /***************************************/
		// // Load Tables
		// tamCustomer = countLines("/home/flav/mestrado/BloomJoin/tbl/customer.tbl");
		// c_customer = malloc(tamCustomer*sizeof(column_customer));
		// readCustomerColumn("/home/flav/mestrado/BloomJoin/tbl/customer.tbl", c_customer);

		// tamOrders = countLines(fileName);
		// c_orders = malloc(tamOrders*sizeof(column_orders));
		// readOrdersColumn(fileName, c_orders, sel);

		// //Result Table
		// for (int i=0; i<tamResult; i++)
		// 	t_result[i] = 0.0;

		// printf("Bloom Nested Join\n");
		// printf("-----------------\n");
		// init = clock();
		// nResult=bloomNested(c_customer, c_orders, tamCustomer, tamOrders, t_result, nBuckets);
		// end = clock();
		// printf("%d linhas\n", nResult);
		// printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

		// free(c_orders);
		// free(c_customer);

		// /*************************************/
		// // Bloom Hash Join
		// /*************************************/
		// tamCustomer = countLines("/home/flav/mestrado/BloomJoin/tbl/customer.tbl");
		// c_customer = malloc(tamCustomer*sizeof(column_customer));
		// readCustomerColumn("/home/flav/mestrado/BloomJoin/tbl/customer.tbl", c_customer);

		// tamOrders = countLines(fileName);
		// c_orders = malloc(tamOrders*sizeof(column_orders));
		// readOrdersColumn(fileName, c_orders, sel);

		// //Result Table
		// for (int i=0; i<tamResult; i++)
		// 	t_result[i] = 0.0;

		// printf("Bloom Hash Join\n");
		// printf("-----------------\n");
		// init = clock();
		// nResult=bloomHash(c_customer, c_orders, tamCustomer, tamOrders, t_result, nBuckets);
		// end = clock();
		// printf("%d linhas\n", nResult);
		// printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

		// free(c_orders);
		// free(c_customer);

		// /***********************************/
		// // Cuckoo Join
		// /***********************************/
		// // Load Tables
		// tamCustomer = countLines("/home/flav/mestrado/BloomJoin/tbl/customer.tbl");
		// c_customer = malloc(tamCustomer*sizeof(column_customer));
		// readCustomerColumn("/home/flav/mestrado/BloomJoin/tbl/customer.tbl", c_customer);

		// tamOrders = countLines(fileName);
		// c_orders = malloc(tamOrders*sizeof(column_orders));
		// readOrdersColumn(fileName, c_orders, sel);

		// //Result Table
		// for (int i=0; i<tamResult; i++)
		// 	t_result[i] = 0.0;

		// printf("Cuckoo Join\n");
		// printf("-----------------\n");
		// init = clock();
		// nResult=cuckooHash(c_customer, c_orders, tamCustomer, tamOrders, t_result, CUCKOO_SIZE);
		// end = clock();
		// printf("%d linhas\n", nResult);
		// printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

		// free(c_orders);
		// free(c_customer);

		// /***********************************/
		// // Cuckoo Filter Join
		// /***********************************/
		// Load Tables
		tamCustomer = countLines("/home/flav/mestrado/BloomJoin/tbl/customer.tbl");
		c_customer = malloc(tamCustomer*sizeof(column_customer));
		readCustomerColumn("/home/flav/mestrado/BloomJoin/tbl/customer.tbl", c_customer);

		tamOrders = countLines(fileName);
		c_orders = malloc(tamOrders*sizeof(column_orders));
		readOrdersColumn(fileName, c_orders, sel);

		//Result Table
		for (int i=0; i<tamResult; i++)
			t_result[i] = 0.0;

		// printf("Cuckoo Join\n");
		// printf("-----------------\n");
		// init = clock();
		// nResult=cuckooFilterJoin(c_customer, c_orders, tamCustomer, tamOrders, t_result, CUCKOO_FILTER_SIZE);
		// end = clock();
		// printf("%d linhas\n", nResult);
		// printf("%.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

		// free(c_orders);
		// free(c_customer);

		//Hash testing
		char str[10];
		for (int i=0; i<tamCustomer; i++)
		{
			sprintf(str, "%d", c_customer[i].C_CUSTKEY);

			printf("Valor: %d \n 	Murmur3 16: %d Murmur3: %d\n 	FNV1a: %d\n 	FNV1A 8: %d\n", c_customer[i].C_CUSTKEY, murmurhash316(str, strlen(str), tamOrders) & (nBuckets-1), murmurhash3(str, strlen(str), tamOrders) & (nBuckets-1), fnv1a(str) & (nBuckets-1), fnv1a8(c_customer[i].C_CUSTKEY) & (nBuckets-1));

			// printf("Valor: %d \n 	Mumur2Desloc: %d Murmur2: %d\n 	Murmur3Desloc: %d Murmur3: %d\n 	Jenkins Desloc: %d Jenkins: %d\n 	Pearson Desloc: %d Pearson: %d\n 	FNV1a: %d\n 	Elf: %d\n", c_customer[i].C_CUSTKEY, (int) HASH_INDEX0, (int)CUCKOO_H1, HASH_INDEX3, (int) CUCKOO_H2,(int) HASH_INDEX1, (int)(hash_jenkins(str) & (nBuckets-1)), (int) HASH_INDEX2, (int) (hash_pearson(str) & (nBuckets-1)), (int)fnv1a(str) & (nBuckets-1), (int) ElfHash(str) & (nBuckets-1));
		}
	}
	else
	{
		tamCustomer = countLines("/home/flav/mestrado/BloomJoin/tbl/customer.tbl");
		c_customer = malloc(tamCustomer*sizeof(column_customer));
		readCustomerColumn("/home/flav/mestrado/BloomJoin/tbl/customer.tbl", c_customer);

		tamOrders = countLines(fileName);
		c_orders = malloc(tamOrders*sizeof(column_orders));
		readOrdersColumn(fileName, c_orders, sel);

		//Result Table
		for (int i=0; i<tamResult; i++)
			t_result[i] = 0.0;

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