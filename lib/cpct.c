#include "cpct.h"

inline void * cpctInsertOHT(void * bktArray)
{
	uint32_t index0[3];
	uint32_t index1[3];
	uint32_t olderCuckoo, aux;

	char str[10];

	ptrBucketArray bucketArray = (ptrBucketArray) bktArray;

	sprintf(str, "%d", bucketArray->olderKey);
	index0[0] = HASH_OHT_CPCT0;

	for (int i=1; i<3; i++)
	{
		if (index0[i-1] == CPCT_OHT_SIZE)
			index0[i] = 0;
		else
			index0[i] = index0[i-1]+1;
	}

	index1[0] = HASH_OHT_CPCT1;
	for (int i=1; i<3; i++)
	{
		if (index1[i-1] == CPCT_OHT_SIZE)
			index1[i] = 0;
		else
			index1[i] = index1[i-1]+1;
	}

	olderCuckoo = bucketArray->olderKey;

	if (OHT0[index0[0]] == bucketArray->olderKey || OHT1[index1[0]] == bucketArray->olderKey || 
		OHT0[index0[1]] == bucketArray->olderKey || OHT1[index1[1]] == bucketArray->olderKey ||
		OHT0[index0[2]] == bucketArray->olderKey || OHT1[index1[2]] == bucketArray->olderKey)
	{
		bucketArray->status = 3;
		return NULL;
	}

	for (int i=0; i<3; i++)
	{
		if (OHT0[index0[i]] == 0)
		{
			OHT0[index0[i]] = olderCuckoo;
			bucketArray->status = 2;
			return NULL;
		}

		if (OHT1[index1[i]] == 0)
		{
			OHT1[index1[i]] = olderCuckoo;
			bucketArray->status = 2;
			return NULL;
		}
	}

	for (int j=0; j<CPCT_OHT_THRESHOLD-1; j++)
	{
		//Table 1
		aux = OHT0[index0[0]];
		OHT0[index0[0]] = olderCuckoo;
		olderCuckoo = aux;
		
		//Table 2
		sprintf(str, "%d", olderCuckoo);
		index1[0] = HASH_OHT_CPCT1;
		for (int i=1; i<3; i++)
		{
			if (index1[i-1] == CPCT_OHT_SIZE)
				index1[i] = 0;
			else
				index1[i] = index1[i-1]+1;
		}

		if ((OHT1[index1[0]] == olderCuckoo) || (OHT1[index1[1]] == olderCuckoo) || (OHT1[index1[2]] == olderCuckoo))
		{
			bucketArray->status = 3;
			return NULL;
		}

		for (int i=0; i<3; i++)
		{
			if (OHT1[index1[i]] == 0)
			{
				OHT1[index1[i]] = olderCuckoo;
				bucketArray->status = 2;
				return NULL;
			}
		}

		aux = OHT1[index1[0]];
		OHT0[index1[0]] = olderCuckoo;
		olderCuckoo = aux;
		
		sprintf(str, "%d", olderCuckoo);
		index0[0] = HASH_OHT_CPCT0;

		for (int i=1; i<3; i++)
		{
			if (index0[i-1] == CPCT_OHT_SIZE)
				index0[i] = 0;
			else
				index0[i] = index0[i-1]+1;
		}

		if ((OHT0[index0[0]] == olderCuckoo) || (OHT0[index0[1]] == olderCuckoo) || (OHT0[index0[2]] == olderCuckoo))
		{
			bucketArray->status = 3;
			return NULL;
		}

		for (int i=0; i<3; i++)
		{
			if (OHT0[index0[i]] == 0)
			{
				OHT0[index0[i]] = olderCuckoo;
				bucketArray->status = 2;
				return NULL;
			}

		}
	}
	bucketArray->status = 0;
	pthread_exit(NULL);
	return NULL;
}

inline int cpctInsertConciseTable(unsigned int key)
{
	char str[10];
	sprintf(str, "%d", key);

	// uint32_t olderKey = key;

	uint32_t fingerPrint;
	uint32_t found;

	pthread_t thread;
	// uint32_t olderCuckoo, aux;

	//Filter Phase
	uint32_t index0;
	uint32_t iIndex0;
	uint32_t tIndex0;
	
	uint32_t index1;
	uint32_t iIndex1;
	uint32_t tIndex1;

	index0 = HASH_FILTER0_CPCT;
	fingerPrint = HASH_FINGERPRINT;
	sprintf(str, "%d", fingerPrint);
	index1 = HASH_FILTER1_CPCT;

	found = 0;
	for (iIndex0=0; iIndex0<5 && found==0; iIndex0++)
	{
		if (((filter0[index0] << ((4-iIndex0)*CPCT_FINGERPRINT_SIZE) >> 56)) == fingerPrint)
		{
			found=1;
			tIndex0 = ((filter0[index0]<<40)>>40) - (popCount((filter0[index0]<<((5-iIndex0)*CPCT_FINGERPRINT_SIZE))>>((5-iIndex0)*CPCT_FINGERPRINT_SIZE)));
			if (cuckooTable0[tIndex0] == key)
				return 3;
			if (cuckooTable0[tIndex0] == 0)
			{
				cuckooTable0[tIndex0] = key;
				return 1;
			}
		}
	}
	found = 0;
	for (iIndex1=0; iIndex1<5 && found==0; iIndex1++)
	{
		if (((filter1[index1] << ((4-iIndex1)*CPCT_FINGERPRINT_SIZE) >> 56)) == fingerPrint)
		{
			found=1;
			tIndex1 = ((filter1[index1]<<40)>>40) - (popCount((filter1[index1]<<((5-iIndex1)*CPCT_FINGERPRINT_SIZE))>>((5-iIndex1)*CPCT_FINGERPRINT_SIZE)));
			if (cuckooTable1[tIndex1] == key)
				return 3;
			if (cuckooTable1[tIndex1] == 0)
			{
				cuckooTable1[tIndex1] = key;
				return 1;
			}
		}
	}

	// olderCuckoo = olderKey;
	// for (int i=0; i<CPCT_THRESHOLD; i++)
	// { 
	// 	if ((((filter0[index0] << ((4-iIndex0)*CPCT_FINGERPRINT_SIZE) >> 56)) == ((uint64_t)fingerPrint)))
	// 	{
	// 		if (cuckooTable0[tIndex0] == olderCuckoo)
	// 			return 3;

	// 		if (cuckooTable0[tIndex0] == 0)
	// 		{
	// 			cuckooTable0[tIndex0] = olderCuckoo;
	// 			return 1;
	// 		}
		
	// 		aux = cuckooTable0[tIndex0];
	// 		cuckooTable0[tIndex0] = olderCuckoo;
	// 		olderCuckoo = aux;
			
	// 		key = olderCuckoo;
	// 		fingerPrint = HASH_FINGERPRINT;
	// 		sprintf(str, "%d", fingerPrint);
	// 		index1 = HASH_FILTER1_CPCT;

	// 		found = 0;
	// 		for (iIndex1=0; iIndex1<5 && found==0; iIndex1++)
	// 			if (((filter0[index0] << ((4-iIndex1)*CPCT_FINGERPRINT_SIZE) >> 56)) == fingerPrint)
	// 		tIndex1 = ((filter1[index1]<<40)>>40) - (popCount((filter1[index1]<<((5-iIndex1)*CPCT_FINGERPRINT_SIZE))>>((5-iIndex1)*CPCT_FINGERPRINT_SIZE)));
	// 	}

	// 	if ((((filter1[index1] << ((4-iIndex1)*CPCT_FINGERPRINT_SIZE) >> 56)) == ((uint64_t)fingerPrint)))
	// 	{
			
	// 		if (cuckooTable1[tIndex1] == olderCuckoo)
	// 			return 3;

	// 		if (cuckooTable1[tIndex1] == 0)
	// 		{
	// 			cuckooTable1[tIndex1] = olderCuckoo;
	// 			return 1;
	// 		}
			
	// 		aux = cuckooTable1[tIndex1];
	// 		cuckooTable1[tIndex1] = olderCuckoo;
	// 		olderCuckoo = aux;
			
	// 		key = olderCuckoo;
	// 		fingerPrint = HASH_FINGERPRINT;
	// 		sprintf(str, "%d", fingerPrint);
	// 		index0 = index1;
	// 		index0 = HASH_FILTER1_CPCT;
				
	// 		found = 0;
	// 		for (iIndex0=0; iIndex0<5 && found==0; iIndex0++)
	// 			if (((filter0[index0] << ((4-iIndex0)*CPCT_FINGERPRINT_SIZE) >> 56)) == fingerPrint)
	// 				found=1;
	// 		tIndex0 = ((filter0[index0]<<40)>>40) - (popCount((filter0[index0]<<((5-iIndex0)*CPCT_FINGERPRINT_SIZE))>>((5-iIndex0)*CPCT_FINGERPRINT_SIZE)));
	// 	}
	// }

	ptrBucketArray bucketArray = (ptrBucketArray) malloc(sizeof(BucketArray));
	bucketArray->olderKey = key;
	bucketArray->status = 3;
	pthread_create(&thread, NULL, cpctInsertOHT, (void *) bucketArray);

	return bucketArray->status;
}

inline int cpctInsertFilter(unsigned int key)
{
	char str[10];
	sprintf(str, "%d", key);

	uint32_t olderCuckoo;
	uint32_t aux;
	uint16_t bigDesloc;
	uint16_t littleDesloc;

	uint32_t index0;
	uint32_t index1;

	//Filter Phase
	// uint32_t index0;
	uint32_t iIndex0;
	
	// uint32_t index1;
	uint32_t iIndex1;

	index0 = HASH_FILTER0_CPCT;
	olderCuckoo = HASH_FINGERPRINT_CPCT;
	sprintf(str, "%d", olderCuckoo);
	index1 = HASH_FILTER1_CPCT;

	for (int i=0; i<5; i++)
		if ((((filter0[index0] << ((4-i)*CPCT_FINGERPRINT_SIZE) >> 56)) == ((uint64_t)olderCuckoo) ||
			(((filter1[index1] << ((4-i)*CPCT_FINGERPRINT_SIZE) >> 56)) == ((uint64_t)olderCuckoo))))
			return 3;

	//Possui entry vazia?
	iIndex0 = (uint32_t)((filter0[index0] << 61) >> 61);
	if (iIndex0 < 5)
	{
		iIndex0--;
		if ((((filter0[index0] << ((4-iIndex0)*CCT_FINGERPRINT_SIZE) >> 56)) == 0))
		{
			filter0[index0] = filter0[index0]
						| (((uint64_t)olderCuckoo) << ((iIndex0*CCT_FINGERPRINT_SIZE)+24));
			filter0[index0] = filter0[index0] + 1;
		}
	}

	iIndex1 = (uint32_t)((filter1[index1] << 61) >> 61);
	if (iIndex1 < 5)
	{
		iIndex1--;
		if ((((filter1[index1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE) >> 56)) == 0))
		{
			filter1[index1] = filter1[index1]
					| (((uint64_t)olderCuckoo) << ((iIndex1*CCT_FINGERPRINT_SIZE)+24));
			filter1[index1] = filter1[index1] + 1;
			return 1;
		}
	}

	//Laço de realocações (renesting)
	for (int j=0; j<CCT_THRESHOLD; j++)
	{
		//Always insert on the first table
		iIndex0 = ((uint32_t) clock()) % 5;

		aux = (filter0[index0] << ((4-iIndex0)*CCT_FINGERPRINT_SIZE)) >> 56;
		bigDesloc = (4+iIndex0)*CCT_FINGERPRINT_SIZE;
		littleDesloc = (5-iIndex0)*CCT_FINGERPRINT_SIZE;
		if (iIndex0 == 4)
			filter0[index0] = ((filter0[index0] << littleDesloc) >> littleDesloc) 
				| (((uint64_t)olderCuckoo) << ((iIndex0*CCT_FINGERPRINT_SIZE)+24)) 
				| ((filter0[index0] >> bigDesloc) << bigDesloc);
		else
			filter0[index0] = ((filter0[index0] << littleDesloc) >> littleDesloc) 
				| (((uint64_t)olderCuckoo) << ((iIndex0*CCT_FINGERPRINT_SIZE)+24)) 
				| ((filter0[index0] >> bigDesloc) << bigDesloc);
		olderCuckoo = aux;

		sprintf(str, "%d", olderCuckoo);
		index1 = HASH_FILTER1_CPCT;

		iIndex1 = ((filter1[index1] << 61) >> 61)-1;
		if (iIndex1 < 5)
		{
			if ((((filter1[index1] << ((5-iIndex1)*CCT_FINGERPRINT_SIZE) >> 56)) == 0))
			{
				filter1[index1] = filter1[index1]
						| (((uint64_t)olderCuckoo) << ((iIndex1*CCT_FINGERPRINT_SIZE)+24));
				filter1[index1] = filter1[index1] + 1;
				return 1;
			}
		}

		iIndex1 = ((uint32_t) clock()) % 5;//bucket index inside 32 bits word	
		aux = (filter1[index1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE)) >> 56;
		bigDesloc = (4+iIndex1)*CCT_FINGERPRINT_SIZE;
		littleDesloc = (5-iIndex1)*CCT_FINGERPRINT_SIZE;
		if (iIndex1 == 4)
			filter1[index1] = ((filter1[index1] << littleDesloc) >> littleDesloc) 
				| (((uint64_t)olderCuckoo) << ((iIndex1*CCT_FINGERPRINT_SIZE)+24)) 
				| ((filter1[index1] >> bigDesloc) << bigDesloc);
		else
			filter1[index1] = ((filter1[index1] << littleDesloc) >> littleDesloc) 
				| (((uint64_t)olderCuckoo) << ((iIndex1*CCT_FINGERPRINT_SIZE)+24)) 
				| ((filter1[index1] >> bigDesloc) << bigDesloc);
		olderCuckoo = aux;

		sprintf(str, "%d", olderCuckoo);
		index0 = index1;
		index0 = HASH_FILTER1_CPCT;

		iIndex0 = ((filter0[index0] << 61) >> 61)-1;
		if (iIndex0 < 5)
		{
			if ((((filter0[index0] << ((5-iIndex0)*CCT_FINGERPRINT_SIZE) >> 56)) == 0))
			{
					filter0[index0] = filter0[index0]
							| (((uint64_t)olderCuckoo) << ((iIndex0*CCT_FINGERPRINT_SIZE)+24));
					filter0[index0] = filter0[index0] + 1;
					return 1;
			}
		}
	}
	return 2;
}

// inline uint64_t popCountCPCT(uint64_t bits)
// {
// 	uint64_t count;
// 	bits = bits>>24;
// 	for (count = 0; bits > 0; bits=bits >> 8)
// 		count+= (((bits<<63)>>63) |  ((bits<<62)>>63) | ((bits<<61)>>63) | ((bits<<60)>>63) |
// 	 			((bits<<59)>>63) | ((bits<<58)>>63) | ((bits<<57)>>63) | ((bits<<56)>>63));
// 	return count;
// }

inline void * insertArrayRoutine(void * orders)
{
	ptrOrders c_orders = (ptrOrders) orders;
	for (int i=c_orders->init; i<c_orders->tamOrders+c_orders->init; i++)
	{
		switch (cpctInsertConciseTable(c_orders->c_orders[i].O_CUSTKEY))
		{
			case 0:
				printf("ERROR: Something went wrong while inserting the key %u on the Array\n", c_orders->c_orders[i].O_CUSTKEY);
				break;
			case 1:
				c_orders->cpctHT++;
				break;
			case 2:
				c_orders->cpctOHT++;
				break;
			case 3:
				c_orders->dupHT++;
				break;
		}
	}
	pthread_exit(NULL);
}

inline void generateCPCT(column_orders * c_orders)
{
	pthread_t threads[NUM_THREADS];
	clock_t init, end;
	// uint32_t dup=0;
	uint32_t dupHT=0;

	uint64_t actualPopCounter0;
	uint64_t oldPopCounter0;

	uint64_t actualPopCounter1;
	uint64_t oldPopCounter1;

	uint32_t cpctOHT=0;
	uint32_t cpctHT=0;

	cpctOccFingerprint=0;
	cpctOHTFingerprint=0;
	dup=0;

	ptrOrders orders[NUM_THREADS];
	for (int i=0; i<NUM_THREADS; i++)
		orders[i] = malloc(sizeof(Orders));

	init = clock();
	likwid_markerStartRegion("Initialization");

	//Allocate CHT
	ctSize0 = 0;
	ctSize1 = 0;

	//Initialize OHT and bitmap
	for (int i=0; i<CCT_OHT_SIZE;i++)
	{
		OHT0[i]=0;
		OHT1[i]=0;
	}

	for (int i=0; i<CCT_FILTER_SIZE; i++)
	{
		filter0[i]=0;
		filter1[i]=0;
	}

	likwid_markerStopRegion("Initialization");
	end = clock();
	printf("Initialization: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	init = clock();
	likwid_markerStartRegion("Generation");

	for (int i=0; i<tamOrders; i++)
	{
		switch (cpctInsertFilter(c_orders[i].O_CUSTKEY))
		{
			case 0:
				printf("ERROR: Something went wrong while inserting the key %u on the filter\n", c_orders[i].O_CUSTKEY);
				break;
			case 1:
				cpctOccFingerprint++;
				break;
			case 2:
				cpctOHTFingerprint++;
				break;
			case 3:
				dup++;
				// cctOHTFingerprint++;
				break;
		}	
	}

	//count Population
	actualPopCounter0 = 0;
	actualPopCounter1 = 0;
	for (int i=0; i<CCT_FILTER_SIZE;i++)
	{
		oldPopCounter0 = (filter0[i] << 61) >> 61;
		actualPopCounter0 = actualPopCounter0 + oldPopCounter0;
		filter0[i] = ((filter0[i]>>24)<<24) | actualPopCounter0;

		oldPopCounter1 = (filter1[i] << 61) >> 61;
		actualPopCounter1 = actualPopCounter1 + oldPopCounter1;
		filter1[i] = ((filter1[i]>>24)<<24) | actualPopCounter1;
	}

	ctSize0 = actualPopCounter0;
	ctSize1 = actualPopCounter1;
	cuckooTable0 = malloc(sizeof(uint32_t)*ctSize0);
	cuckooTable1 = malloc(sizeof(uint32_t)*ctSize1);

	for (int i=0; i<ctSize0; i++)
		cuckooTable0[i] = 0;

	for (int i=0; i<ctSize1; i++)
		cuckooTable1[i] = 0;

	for (int i=0; i<NUM_THREADS; i++)
	{
		orders[i]->c_orders = c_orders;
		orders[i]->tamOrders = tamOrders/NUM_THREADS;
		orders[i]->init = i*(tamOrders/NUM_THREADS);
		orders[i]->cpctHT = 0;
		orders[i]->cpctOHT = 0;
		orders[i]->dupHT = 0;
		pthread_create(&threads[i], NULL, insertArrayRoutine, (void *) orders[i]);
	}

	for (int i=0; i<NUM_THREADS; i++)
		pthread_join(threads[i], NULL);

	for (int i=0; i<NUM_THREADS; i++)
	{
		cpctOHT = cpctOHT+orders[i]->cpctOHT;
		cpctHT = cpctHT+orders[i]->cpctHT;
		dupHT = dupHT+orders[i]->dupHT;
	}

	likwid_markerStopRegion("Generation");
	end = clock();
	printf("CHT Generation: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
	printf("OHT has %d occupied buckets and %d on the filter \n", cpctOHT, cpctOHTFingerprint);
	printf("CHT has %d occupied buckets and %d on the filter \n", cpctHT, cpctOccFingerprint);
	// printf("Succesfull insertions on fisrt try Filter %d\n", SUCCFILTER);
	// printf("Succesfull insertions on realocantion Filter %d\n", REALOCFILTER);
}

inline int cpctLookUp(unsigned int key)
{
	char str[10];
	sprintf(str, "%d", key);

	uint32_t fingerPrint;

	uint32_t oIndex0[3];
	uint32_t oIndex1[3];
	uint32_t index0;
	uint32_t index1;

	//Filter Phase
	// uint32_t iIndex0;
	// uint32_t iIndex1;

	uint32_t tIndex0;
	uint32_t tIndex1;

	index0 = HASH_FILTER0_CPCT;
	fingerPrint = HASH_FINGERPRINT;
	sprintf(str, "%d", fingerPrint);
	index1 = HASH_FILTER1_CPCT;

	for (int i=0; i<5; i++)
	{
		if ((((filter0[index0] << ((4-i)*CPCT_FINGERPRINT_SIZE) >> 56)) == ((uint64_t)fingerPrint)) ||
		(((filter1[index1] << ((4-i)*CPCT_FINGERPRINT_SIZE) >> 56)) == ((uint64_t)fingerPrint)))
		{
			tIndex0 = ((filter0[index0]<<40)>>40) - (popCount((filter0[index0]<<((5-i)*CPCT_FINGERPRINT_SIZE))>>((5-i)*CPCT_FINGERPRINT_SIZE)));
			tIndex1 = ((filter1[index1]<<40)>>40) - (popCount((filter1[index1]<<((5-i)*CPCT_FINGERPRINT_SIZE))>>((5-i)*CPCT_FINGERPRINT_SIZE)));

			if ((cuckooTable0[tIndex0] == key) || (cuckooTable1[tIndex1] == key))
				return 1;
		}
	}

	//Search OHT
	sprintf(str, "%d", key);
	oIndex0[0] = HASH_OHT_CPCT0;
	for (int i=1; i<3; i++)
	{
		if (oIndex0[i-1] == CPCT_OHT_SIZE)
			oIndex0[i] = 0;
		else
			oIndex0[i] = oIndex0[i-1]+1;
	}

	oIndex1[0] = HASH_OHT_CPCT1;
	for (int i=1; i<3; i++)
	{
		if (oIndex1[i-1] == CPCT_OHT_SIZE)
			oIndex1[i] = 0;
		else
			oIndex1[i] = oIndex1[i-1]+1;
	}

	if (OHT0[oIndex0[0]] == key || OHT1[oIndex1[0]] == key || 
		OHT0[oIndex0[1]] == key || OHT1[oIndex1[1]] == key ||
		OHT0[oIndex0[2]] == key || OHT1[oIndex1[2]] == key)
			return 1;

	return 0;
}

inline int cpctJoin(column_customer * c_customer, column_orders * c_orders)
{
	clock_t init, end;
	uint32_t index=0;

	SUCCFILTER=0;
	REALOCFILTER=0;

	generateCPCT(c_orders);

	init=clock();
	likwid_markerStartRegion("Core");
	for (int i=0; i<tamCustomer; i++)
	{
		if (cpctLookUp(c_customer[i].C_CUSTKEY) == 0)
			index++;
	}
	likwid_markerStopRegion("Core");
	end=clock();

	printf("Join core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	return index;
}