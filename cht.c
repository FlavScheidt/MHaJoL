#include "cht.h"

inline void generateCHT(column_orders *c_orders, int tamOrders, CHT * cht)
{
	clock_t init, end;
	uint32_t ohtOcc=0;
	init = clock();
	
	likwid_markerStartRegion("Initialization");
	//Allocate CHT
	cht->tamHT = 0;

	//Initialize OHT and bitmap
	for (int i=0; i<CHT_OHT_SIZE;i++)
	{
		cht->OHT[i]=-1;
		cht->bitmap[i]=0;
	}

	likwid_markerStopRegion("Initialization");
	end = clock();
	printf("Initialization: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	init = clock();
	likwid_markerStartRegion("Generation");
	for (int i=0; i<tamOrders; i++)
		if (chtInsertBitmap(c_orders[i].O_CUSTKEY, tamOrders, cht) == 0)
			printf("ERROR: Something went wrong while inserting the key %u on the CHT", c_orders[i].O_CUSTKEY);

	cht->HT = malloc(sizeof(uint32_t)*cht->tamHT);
	for (int i=0; i<=cht->tamHT; i++)
		cht->HT[i] = 0;

	for (int i=0; i<tamOrders; i++)
		if (chtInsertConciseTable(c_orders[i].O_CUSTKEY, cht, tamOrders) == 0)
			ohtOcc++;

	likwid_markerStopRegion("Generation");
	end = clock();
	printf("CHT Generation: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
	printf("OHT has %d occupied buckets\n", ohtOcc);
}

inline int chtInsertBitmap(unsigned int key, int tamOrders, CHT * cht)
{
	char str[10];
	sprintf(str, "%d", key);

	//Bitmap Phase
	uint32_t index = CHT_HASH; //logical index
	uint32_t vIndex = index/32; //vector index
	uint32_t wIndex = (index-(vIndex*32))+32; //word index

	uint32_t iIndex = wIndex; //Insertion index

	uint64_t one = 1;
	uint32_t collisions = 0;

	while (((cht->bitmap[vIndex] << (iIndex-64)) >> 63 == 1)) 
	//If I need to enter this loop, there is a collision!
	{
		collisions++;

		//OHT Phase
		//If there is too many collisions, insert into the OHT
		if (collisions > CHT_THRESHOLD)
		{
			index = CHT_OHT_HASH;
			for (int i=index; i<CHT_OHT_SIZE; i++)
			{
				if (cht->OHT[index] != 0)
				{
					cht->OHT[index] = key;
					return 1;
				}
			}
			return 0;
		}

		//Not enough collisions for the OHT, go to the next position
		if (iIndex+1 == 63)
		{
			vIndex=vIndex+1;
			iIndex=0;
		}
		else
			iIndex++;

	}

	//Insert on the bitmap! Happy case =)
	cht->bitmap[vIndex] = (one << iIndex) | cht->bitmap[vIndex];
	cht->tamHT++;

	//Computates de prefix pop count
	cht->bitmap[vIndex]++;

	return 1;
}

inline int chtInsertConciseTable(unsigned int key, CHT * cht, int tamOrders)
{
	char str[10];
	sprintf(str, "%d", key);

	uint32_t index = CHT_HASH; //logical index
	uint32_t vIndex = index/32; //vector index

	uint32_t htIndex = 0;
	uint32_t collisions = 0;

	//Calculate position using the bitmap counters
	for (int i=0; i<vIndex; i++)
		htIndex+=((uint32_t) cht->bitmap[i]);

	//Probe
	while (cht->HT[htIndex] == 0)
	{
		if (collisions > CHT_THRESHOLD)
			return 0;

		htIndex++;
		collisions++;
	}

	cht->HT[htIndex] = key;
	return 1;

}

inline int chtLookUp(unsigned int key, CHT *cht)
{
	char str[10];
	sprintf(str, "%d", key);

	uint32_t index = CHT_HASH; //logical index
	uint32_t vIndex = index/32; //vector index
	uint32_t wIndex = (index-(vIndex*32))+32; //word index
	uint32_t iIndex = wIndex; //Insertion index


	if ((cht->bitmap[vIndex] << (iIndex-64)) >> 63 == 0)
		return 0;

	uint32_t htIndex = 0;
	uint32_t collisions = 0;

	//Calculate position using the bitmap counters
	for (int i=0; i<vIndex; i++)
		htIndex+=((uint32_t) cht->bitmap[i]);

	//Probe
	while (collisions <= CHT_THRESHOLD)
	{
		if (cht->HT[htIndex] == key)
			return 1;

		htIndex++;
		collisions++;
	}

	//Search OHT
	index = CHT_OHT_HASH;
	for (int i=index; i<CHT_OHT_SIZE; i++)
		if (cht->OHT[index] == key)
			return 1;

	return 0;
}

inline int chtJoin(column_customer * c_customer, column_orders * c_orders, int tamCustomer, int tamOrders, float * t_result, int nBuckets)
{
	clock_t init, end;
	uint32_t index=0;

	CHT cht;

	generateCHT(c_orders, tamOrders, &cht);

	init=clock();
	likwid_markerStartRegion("Core");

	for (int i=0; i<tamCustomer; i++)
	{
		if (chtLookUp(c_customer[i].C_CUSTKEY, &cht) == 0)
		{
			t_result[index] = c_customer[i].C_ACCTBAL;
			index++;
		}
	}
	likwid_markerStopRegion("Core");
	end=clock();

	printf("Join core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	return index;
}