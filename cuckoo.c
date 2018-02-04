#include "cuckoo.h"
/***************************************************
	Cuckoo Hash and Cuckoo Filter Implementations
***************************************************/
int cuckooInsert(unsigned int key, int *cTable1, int *cTable2, int tamOrders, int nBuckets)
{
	char str[10];
	int index1, index2;
	unsigned int olderCuckoo;
	int aux;

	sprintf(str, "%d", key);
	index1 = CUCKOO_H1;
	index2 = CUCKOO_H2;

	if (cuckooLookUp(key, index1, index2, cTable1, cTable2) > 0)
		return 1;

	olderCuckoo = key;

	for (int j=0; j<CUCKOO_MAX_TRY; j++)
	{
		//Table 1
		if (cTable1[index1] == -1)
		{
			cTable1[index1] = olderCuckoo;
			return 1;
		}

		aux = cTable1[index1];
		cTable1[index1] = olderCuckoo;
		olderCuckoo = aux;

		//Table 2
		sprintf(str, "%d", olderCuckoo);
		index2 = CUCKOO_H2;

		if (cTable2[index2] == -1)
		{
			cTable2[index2] = olderCuckoo;
			return 1;
		}

		aux = cTable2[index2];
		cTable2[index2] = olderCuckoo;
		olderCuckoo = aux;

		sprintf(str, "%d", olderCuckoo);
		index1 = CUCKOO_H1;
		index2 = CUCKOO_H2;
	}
	return olderCuckoo;
}

int cuckooLookUp(unsigned int key, int index1, int index2, int * cTable1, int *cTable2)
{
	if (cTable1[index1] == key || cTable2[index2] == key)
		return 1;

	return 0;
}

void generateCuckooTable(column_orders *c_orders, int tamOrders, int nBuckets, int * cTable1, int * cTable2)
{
	clock_t init, end;
	init = clock();

	for (int b=0; b<nBuckets; b++)
	{
		cTable1[b] = -1;
		cTable2[b] = -1;
	}

	end = clock();
	printf("Initialization: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	init = clock();
	for (int i=0; i<tamOrders; i++)
	{
		if (cuckooInsert(c_orders[i].O_CUSTKEY, cTable1, cTable2, tamOrders, nBuckets) != 1)
		{
			printf("	Realocating cuckoo table... it may take a while\n");
			//generateCuckooTable(c_orders, tamOrders, nBuckets, cTable1, cTable2);
			//end = clock();
			//printf("Cuckoo Table Generation: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
		}
	}
	end = clock();
	printf("Cuckoo Table Generation: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
}


int cuckooHash(column_customer * c_customer, column_orders * c_orders, int tamCustomer, int tamOrders, float * t_result, int nBuckets)
{
	char str[10];
	clock_t init, end;
	int ocupation = 0;

	int index = 0;
	int index1, index2;

	nBuckets = nBuckets/2;

	int * cTable1 = malloc((nBuckets)*sizeof(int));
	int * cTable2 = malloc((nBuckets)*sizeof(int));

	generateCuckooTable(c_orders, tamOrders, nBuckets, cTable1, cTable2);

	for (int j=0; j<nBuckets; j++)
	{
		if (cTable1[j] != -1)
			ocupation++;
		
		if (cTable2[j] != -1)
			ocupation++;
	}

	init=clock();
	for (int i=0; i<tamCustomer; i++)
	{
		sprintf(str, "%d", c_customer[i].C_CUSTKEY);
		index1 = CUCKOO_H1;
		index2 = CUCKOO_H2;

		if (cuckooLookUp(c_customer[i].C_CUSTKEY, index1, index2, cTable1, cTable2) == 0)
		{
			t_result[index] = c_customer[i].C_ACCTBAL;
			index++;
		}
	}
	end=clock();

	printf("Join core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	printf("Cuckoo Ocupation: %.f%% \n", (double)(ocupation*100)/(nBuckets*2));

	return index;
}

/************************************************
	CUCKOO FILTER
*************************************************/

int cuckooFilterLookUp(int key, uint8_t fingerPrint, uint16_t index1, bucket * nest)
{
	uint16_t index2;
	char str[10];
	int nBuckets = CUCKOO_FILTER_SIZE;

	sprintf(str, "%d", fingerPrint);
	index2 = index1 ^ CUCKOO_H2;

	// if (((nest[index1] << 24) >> 24 == fingerPrint) || ((nest[index1] << 16) >> 24 == fingerPrint) || ((nest[index1] << 8) >> 24 == fingerPrint) || ((nest[index1] >> 24) == fingerPrint) || ((nest[index2] << 24) >> 24 == fingerPrint) || ((nest[index2] << 16) >> 24 == fingerPrint) || ((nest[index2] << 8) >> 24 == fingerPrint) || ((nest[index2] >> 24) == fingerPrint))
	// 	printf("fingerPrint: %d, index1 %d %d %d %d index2 %d %d %d %d\n", fingerPrint, (nest[index1] << 24) >> 24 , (nest[index1] << 16) >> 24, (nest[index1] << 8) >> 24, (nest[index1] >> 24), (nest[index2] << 24) >> 24, (nest[index2] << 16) >> 24, (nest[index2] << 8) >> 24, (nest[index2] >> 24));

	return (((nest[index1] << 24) >> 24 == fingerPrint) || ((nest[index1] << 16) >> 24 == fingerPrint) || ((nest[index1] << 8) >> 24 == fingerPrint) || ((nest[index1] >> 24) == fingerPrint) || ((nest[index2] << 24) >> 24 == fingerPrint) || ((nest[index2] << 16) >> 24 == fingerPrint) || ((nest[index2] << 8) >> 24 == fingerPrint) || ((nest[index2] >> 24) == fingerPrint));

}

int cuckooFilterInsert(int key, bucket * nest, int nBuckets)
{
	char str[10];

	sprintf(str, "%d", key);

	uint8_t fingerPrint = CUCKOO_FINGERPRINT;
	uint16_t index = CUCKOO_H2;
	uint16_t i;

	unsigned char *ptr;

	uint8_t olderCuckoo;

	srand(time(NULL));

	if (cuckooFilterLookUp(key, fingerPrint, index, nest) == 1)
		return 1;

	for (int n=0; n<CUCKOO_FILTER_MAX_TRY; n++)
	{
		if ((nest[index] << 24) >> 24 == 0) //está vazio, entao eh inserido
		{
			nest[index] = nest[index] | fingerPrint;
			return 1;
		}
		if ((nest[index] << 16) >> 24 == 0) //está vazio, entao eh inserido
		{
			nest[index] = nest[index] | (((uint32_t) fingerPrint) << 8);
			return 1;
		}
		if ((nest[index] << 8) >> 24 == 0) //está vazio, entao eh inserido
		{
			nest[index] = nest[index] | (((uint32_t) fingerPrint) << 16);

			return 1;
		}
		if ((nest[index]) >> 24 == 0) //está vazio, entao eh inserido
		{
			nest[index] = nest[index] | (((uint32_t)fingerPrint) << 24);
			return 1;
		}
		
		//A chave a ser substituída é sorteada
		i = rand() % 4;

		ptr = (unsigned char *) &nest[index];
		ptr += i;

		olderCuckoo = (uint8_t) *ptr;
		*ptr = 0;

		nest[index] = nest[index] & ((uint32_t)(fingerPrint) << 8*(3-i));

		fingerPrint = olderCuckoo;

		sprintf(str, "%d", fingerPrint);
		index = index ^ CUCKOO_H2;

	}
	return 0;

}

void cuckooFilterGeneration(column_orders *c_orders, int tamOrders, bucket * nest)
{
	clock_t init, end;
	init = clock();

	for (int b=0; b<CUCKOO_FILTER_SIZE; b++)
		nest[b] = 0;

	end = clock();
	printf("Initialization: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	init = clock();
	for (int i=0; i<tamOrders; i++)
	{
		if (cuckooFilterInsert(c_orders[i].O_CUSTKEY, nest, CUCKOO_FILTER_SIZE) == 0)
			printf("Not inserted\n");
	}

	end = clock();
	printf("Cuckoo Filter Generation: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

}

int cuckooFilterJoin(column_customer * c_customer, column_orders * c_orders, int tamCustomer, int tamOrders, float * t_result, int nBuckets)
{
	int index = 0;
	uint16_t index1;
	uint8_t fingerPrint;
	int ocupation = 0;
	clock_t init, end;
	char str[10];
	int key;
	bucket * nest = malloc(CUCKOO_FILTER_SIZE*32);

	cuckooFilterGeneration(c_orders, tamOrders, nest);

	for (int j=0; j<nBuckets; j++)
		if (nest[j] !=0 )
			ocupation++;

	init=clock();
	for (int i=0; i<tamCustomer; i++)
	{
		sprintf(str, "%d", c_customer[i].C_CUSTKEY);
		key = c_customer[i].C_CUSTKEY;
		fingerPrint = CUCKOO_FINGERPRINT;
		index1 = CUCKOO_H2;

		if (cuckooFilterLookUp(c_customer[i].C_CUSTKEY, fingerPrint, index1, nest) == 0)
		{
			t_result[index] = c_customer[i].C_ACCTBAL;
			index++;
		}
	}
	end=clock();

	printf("Join core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	printf("Cuckoo Ocupation: %.f%% \n", (double)(ocupation*100)/(nBuckets*2));

	return index;
}