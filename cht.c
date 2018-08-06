// #include "cht.h"

// inline void generateCHT(column_orders * c_orders, int tamOrders, CHT * cht)
// {
// 	clock_t init, end;
// 	uint32_t ohtOcc=0;
// 	uint32_t chtOcc=0;
// 	uint32_t ohtOccBMP=0;
// 	uint32_t chtOccBMP=0;
// 	uint32_t dup=0;

// 	uint64_t actualPopCounter;
// 	uint64_t oldPopCounter;

// 	init = clock();
	
// 	likwid_markerStartRegion("Initialization");
// 	//Allocate CHT
// 	cht->tamHT = 0;

// 	//Initialize OHT and bitmap
// 	for (int i=0; i<CHT_OHT_SIZE;i++)
// 		cht->OHT[i]=0;

// 	for (int i=0; i<CHT_BMP_SIZE; i++)
// 		cht->bitmap[i]=0;

// 	likwid_markerStopRegion("Initialization");
// 	end = clock();
// 	printf("Initialization: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

// 	init = clock();
// 	likwid_markerStartRegion("Generation");

// 	for (int i=0; i<tamOrders; i++)
// 	{
// 		switch (chtInsertBitmap(c_orders[i].O_CUSTKEY, tamOrders, cht))
// 		{
// 			case 0:
// 				printf("ERROR: Something went wrong while inserting the key %u on the CHT\n", c_orders[i].O_CUSTKEY);
// 				break;
// 			case 1:
// 				chtOccBMP++;
// 				break;
// 			case 2:
// 				ohtOccBMP++;
// 				break;
// 			case 3:
// 				dup++;
// 				ohtOccBMP++;
// 				break;
// 		}	
// 	}

// 	//count Population
// 	actualPopCounter = 0;
// 	for (int i=0; i<CHT_BMP_SIZE;i++)
// 	{
// 		oldPopCounter = popCount(cht->bitmap[i]>>32);
// 		actualPopCounter = actualPopCounter + oldPopCounter;
// 		cht->bitmap[i] = ((cht->bitmap[i]>>32)<<32) + actualPopCounter;
// 	}

// 	cht->tamHT = actualPopCounter;

// 	cht->HT = malloc(sizeof(uint32_t)*cht->tamHT);

// 	for (int i=0; i<cht->tamHT; i++)
// 		cht->HT[i] = 0;

// 	for (int i=0; i<tamOrders; i++)
// 	{
// 		if (chtInsertConciseTable(c_orders[i].O_CUSTKEY, cht, tamOrders) == 0)
// 			ohtOcc++;
// 		else
// 			chtOcc++;
// 	}

// 	likwid_markerStopRegion("Generation");
// 	end = clock();
// 	printf("CHT Generation: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
// 	printf("OHT has %d occupied buckets and %d on the bitmap \n", ohtOcc, ohtOccBMP);
// 	printf("CHT has %d occupied buckets and %d on the bitmap %d \n", chtOcc, chtOccBMP, cht->tamHT);
// 	printf("Duplicates %d\n", dup);
// }

// inline int chtInsertBitmap(unsigned int key, int tamOrders, CHT * cht)
// {
// 	char str[10];
// 	sprintf(str, "%d", key);

// 	int counter;
// 	//Bitmap Phase
// 	uint32_t index = CHT_HASH; //logical index
// 	uint32_t vIndex = (uint32_t) index/32; //vector index
// 	uint32_t bkt = index%31; //bucket index inside 32 bits word
// 	uint32_t iIndex = bkt; //Insertion index
// 	// printf("bkt %u ",bkt);

// 	// printf("KEY: %d  INDEX: %d  VINDEX: %d BITMAP %lu VALUE: %lu \n", key, index, vIndex, cht->bitmap[vIndex], ((cht->bitmap[vIndex] << (63-iIndex)) >> 63));

// 	uint64_t one = 1;
// 	uint32_t collisions = 0;

// 	while (collisions < CHT_THRESHOLD)
// 	{
// 		if (((uint32_t) ((cht->bitmap[vIndex]<<32)>>32) < 32) && (((cht->bitmap[vIndex] << (31-iIndex)) >> 63) == 0))
// 		{
// 			//Insert on the bitmap! Happy case =)
// 			cht->bitmap[vIndex] = cht->bitmap[vIndex] | (one << (iIndex+32));
// 			return 1;
// 		}
// 		else
// 		{
// 			//Not enough collisions for the OHT, go to the next position
// 			if (iIndex >= 63)
// 			{
// 				if (vIndex >= CHT_BMP_SIZE)
// 					vIndex=0;
// 				else
// 					vIndex++;

// 				iIndex=32;
// 			}
// 			else
// 				iIndex++;
// 		}

// 		collisions++;
// 	}

// 	index = CHT_OHT_HASH;
// 	counter = 0;
// 	while (counter < CHT_OHT_SIZE)
// 	{
// 		if (cht->OHT[index] == key)
// 			return 3;

// 		if (cht->OHT[index] == 0)
// 		{
// 			cht->OHT[index] = key;
// 			return 2;
// 		}

// 		if (index < CHT_OHT_SIZE)
// 			index++;
// 		else
// 			index=0;

// 		counter++;
// 	}

// 	return 0;
// }

// inline uint64_t popCount(uint64_t bits)
// {
// 	uint64_t count;
// 	for (count = 0; bits > 0; bits &= (bits-1))
// 		count++;
// 	return count;
// }

// inline int chtInsertConciseTable(unsigned int key, CHT * cht, int tamOrders)
// {
// 	char str[10];
// 	sprintf(str, "%d", key);

// 	uint32_t index = CHT_HASH; //logical index
// 	uint32_t vIndex = (uint32_t) index/32; //vector index
// 	uint32_t bkt = index%31; //bucket index inside 32 bits word

// 	uint32_t pos;
// 	uint32_t bitsUp;
// 	uint32_t zero = 0;

// 	uint32_t htIndex = 0;
// 	uint32_t collisions = 0;

// 	//Calculates the position
// 	//BitsUp is the # of 1`s on the word until the bucket
// 	bitsUp = ((uint32_t)(cht->bitmap[vIndex] >> 32)) & (~((~zero)>>(31-bkt)));
// 	pos = abs(((uint32_t)popCount(bitsUp))-((uint32_t)(cht->bitmap[vIndex])));

// 	htIndex=pos;
// 	// 	printf("KEY %u INDEX %u POPCOUNTER %u bitmap " PRINTF_BINARY_PATTERN_INT64 "\n", key, index, (uint32_t) cht->bitmap[vIndex], PRINTF_BYTE_TO_BINARY_INT64(cht->bitmap[vIndex]));
// 	// printf("bitmap " PRINTF_BINARY_PATTERN_INT64 ", vindex %u bkt %u bitsUp "PRINTF_BINARY_PATTERN_INT32" numBitsUp %lu pos %u\n", PRINTF_BYTE_TO_BINARY_INT64(cht->bitmap[vIndex]), vIndex,  bkt, PRINTF_BYTE_TO_BINARY_INT32(bitsUp), popCount(bitsUp), pos);

// 	//Probe
// 	while ((collisions < CHT_THRESHOLD))
// 	{
// 		if (cht->HT[htIndex] == 0)
// 		{
// 			cht->HT[htIndex] = key;
// 			return 1;
// 		}

// 		if (htIndex > cht->tamHT)
// 			htIndex=0;
// 		else
// 			htIndex++;

// 		collisions++;
// 	}

// 	// printf("KEY %u INDEX %u POPCOUNTER %u bitmap " PRINTF_BINARY_PATTERN_INT64 "\n", key, index, (uint32_t) cht->bitmap[vIndex], PRINTF_BYTE_TO_BINARY_INT64(cht->bitmap[vIndex]));
// 	// printf("bitmap " PRINTF_BINARY_PATTERN_INT64 ", vindex %u bkt %u bitsUp "PRINTF_BINARY_PATTERN_INT32" numBitsUp %lu pos %u\n", PRINTF_BYTE_TO_BINARY_INT64(cht->bitmap[vIndex]), vIndex,  bkt, PRINTF_BYTE_TO_BINARY_INT32(bitsUp), popCount(bitsUp), pos);

// 	return 0;
// }

// inline int chtLookUp(unsigned int key, CHT *cht)
// {
// 	char str[10];

// 	uint32_t index = CHT_HASH; //logical index
// 	uint32_t vIndex = index/31; //vector index
// 	uint32_t bkt = index%31; //bucket index inside 32 bits word
// 	uint32_t iIndex = bkt; //Insertion index

// 	uint32_t pos;
// 	uint32_t bitsUp;

// 	int counter;
// 	uint32_t zero = 0;

// 	uint32_t htIndex = 0;
// 	uint32_t collisions = 0;

// 	if ((cht->bitmap[vIndex] << (31-iIndex)) >> 63 == 0)
// 		return 0;

// 	//Calculates the position
// 	//BitsUp is the # of 1`s on the word until the bucket
// 	bitsUp = ((uint32_t)(cht->bitmap[vIndex] >> 32)) & (~((~zero)>>(31-bkt)));
// 	pos = abs(((uint32_t)popCount(bitsUp))-((uint32_t)(cht->bitmap[vIndex])));


// 	htIndex=pos;

// 	//Probe
// 	while (collisions < CHT_THRESHOLD)
// 	{
// 		if (cht->HT[htIndex] == key)
// 			return 1;

// 		if (htIndex > cht->tamHT)
// 			htIndex=0;
// 		else
// 			htIndex++;
// 		collisions++;
// 	}

// 	//Search OHT
// 	index = CHT_OHT_HASH;
// 	counter = 0;
// 	while (counter < CHT_OHT_SIZE)
// 	{
// 		if (cht->OHT[index] == key)
// 			return 1;

// 		if (index < CHT_OHT_SIZE)
// 			index++;
// 		else
// 			index=0;

// 		counter++;
// 	}

// 	return 0;
// }

// inline int chtJoin(column_customer * c_customer, column_orders * c_orders, int tamCustomer, int tamOrders, int nBuckets)
// {
// 	clock_t init, end;
// 	uint32_t index=0;

// 	CHT cht;

// 	generateCHT(c_orders, tamOrders, &cht);

// 	init=clock();
// 	likwid_markerStartRegion("Core");
// 	for (int i=0; i<tamCustomer; i++)
// 	{
// 		if (chtLookUp(c_customer[i].C_CUSTKEY, &cht) == 0)
// 			index++;
// 	}
// 	likwid_markerStopRegion("Core");
// 	end=clock();

// 	printf("Join core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

// 	return index;
// }