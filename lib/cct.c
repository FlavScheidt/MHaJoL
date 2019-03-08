#include "cct.h"

inline int cctInsertConciseTable(uint32_t key, int tamOrders)
{
	char str[10];
	sprintf(str, "%d", key);

	uint32_t olderKey = key;

	uint32_t fingerPrint;
	uint32_t aux;

	uint32_t index0, index01, index02;
	uint32_t index1, index11, index12;

	uint32_t olderCuckoo;

	//Filter Phase
	uint32_t vIndex0;
	uint32_t iIndex0;
	uint32_t tIndex0;
	
	uint32_t vIndex1;
	uint32_t iIndex1;
	uint32_t tIndex1;

	index0 = HASH_FILTER0;
	fingerPrint = HASH_FINGERPRINT;
	sprintf(str, "%d", fingerPrint);
	index1 = HASH_FILTER1;

	vIndex0 = (uint32_t) index0/5; //vector index
	iIndex0 = index0%5; //bucket index inside 32 bits word
		
	vIndex1 = (uint32_t) index1/5; //vector index
	iIndex1 = index1%5; //bucket index inside 32 bits word

	tIndex0 = ((filter0CCT[vIndex0]<<40)>>40) - (popCount((filter0CCT[vIndex0]<<((5-iIndex0)*CCT_FINGERPRINT_SIZE))>>((5-iIndex0)*CCT_FINGERPRINT_SIZE)));
	tIndex1 = ((filter1CCT[vIndex1]<<40)>>40) - (popCount((filter1CCT[vIndex1]<<((5-iIndex1)*CCT_FINGERPRINT_SIZE))>>((5-iIndex1)*CCT_FINGERPRINT_SIZE)));

	if ((cuckooTable0[tIndex0] == olderKey) || (cuckooTable1[tIndex1] == olderKey))
		return 3;

	olderCuckoo = olderKey;
	for (int i=0; i<CCT_THRESHOLD; i++)
	{ 
		if ((((filter0CCT[vIndex0] << ((4-iIndex0)*CCT_FINGERPRINT_SIZE) >> 56)) == ((uint64_t)fingerPrint)))
		{
			if (cuckooTable0[tIndex0] == olderCuckoo)
				return 3;
			if (cuckooTable0[tIndex0] == 0)
			{
				cuckooTable0[tIndex0] = olderCuckoo;
				return 1;
			}
		
			aux = cuckooTable0[tIndex0];
			cuckooTable0[tIndex0] = olderCuckoo;
			olderCuckoo = aux;

			key = olderCuckoo;
			fingerPrint = HASH_FINGERPRINT;
			sprintf(str, "%d", fingerPrint);
			index1 = HASH_FILTER1;

			vIndex1 = (uint32_t) index1/5; //vector index
			iIndex1 = index1%5; //bucket index inside 32 bits word

			tIndex1 = ((filter1CCT[vIndex1]<<40)>>40) - (popCount((filter1CCT[vIndex1]<<((5-iIndex1)*CCT_FINGERPRINT_SIZE))>>((5-iIndex1)*CCT_FINGERPRINT_SIZE)));

		}

		if ((((filter1CCT[vIndex1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE) >> 56)) == ((uint64_t)fingerPrint)))
		{
			if (cuckooTable1[tIndex1] == olderCuckoo)
				return 3;
			if (cuckooTable1[tIndex1] == 0)
			{
				cuckooTable1[tIndex1] = olderCuckoo;
				return 1;
			}
			
			aux = cuckooTable1[tIndex1];
			cuckooTable1[tIndex1] = olderCuckoo;
			olderCuckoo = aux;
			
			key = olderCuckoo;
			fingerPrint = HASH_FINGERPRINT;
			sprintf(str, "%d", fingerPrint);
			index0 = index1;
			index0 = HASH_FILTER1;
				
			vIndex0 = (uint32_t) index0/5; //vector index
			iIndex0 = index0%5; //bucket index inside 32 bits word
			tIndex0 = ((filter0CCT[vIndex0]<<40)>>40) - (popCount((filter0CCT[vIndex0]<<((5-iIndex0)*CCT_FINGERPRINT_SIZE))>>((5-iIndex0)*CCT_FINGERPRINT_SIZE)));
		}
	}

	sprintf(str, "%d", olderKey);
	index0 = HASH_OHT_CCT0;
	if (index0 == CCT_OHT_SIZE)
		index01 = 0;
	else
		index01 = index0+1;
	if (index01 == CCT_OHT_SIZE)
		index02 = 0;
	else
		index02 = index01+1;

	index1 = HASH_OHT_CCT1;
	if (index1 == CCT_OHT_SIZE)
		index11 = 0;
	else
		index11 = index1+1;
	if (index11 == CCT_OHT_SIZE)
		index12 = 0;
	else
		index12 = index11+1;
	olderCuckoo = olderKey;

	if (OHT0CCT[index0] == olderKey || OHT1CCT[index1] == olderKey || 
		OHT0CCT[index01] == olderKey || OHT1CCT[index11] == olderKey ||
		OHT0CCT[index02] == olderKey || OHT1CCT[index12] == olderKey)
		return 3;

	if (OHT0CCT[index0] == 0)
	{
		OHT0CCT[index0] = olderCuckoo;
		return 2;
	}

	if (OHT0CCT[index01] == 0)
	{
		OHT0CCT[index01] = olderCuckoo;
		return 2;
	}

	if (OHT0CCT[index02] == 0)
	{
		OHT0CCT[index02] = olderCuckoo;
		return 2;
	}

	if (OHT1CCT[index1] == 0)
	{
		OHT1CCT[index1] = olderCuckoo;
		return 2;
	}

	if (OHT1CCT[index11] == 0)
	{
		OHT1CCT[index11] = olderCuckoo;
		return 2;
	}

	if (OHT1CCT[index12] == 0)
	{
		OHT1CCT[index12] = olderCuckoo;
		return 2;
	}

	for (int j=0; j<CCT_OHT_THRESHOLD-1; j++)
	{
		//Table 1
		aux = OHT0CCT[index0];
		OHT0CCT[index0] = olderCuckoo;
		olderCuckoo = aux;

		//Table 2
		sprintf(str, "%d", olderCuckoo);
		index1 = HASH_OHT_CCT1;
		if (index1 == CCT_OHT_SIZE)
			index11 = 0;
		else
			index11 = index1+1;
		if (index11 == CCT_OHT_SIZE)
			index12 = 0;
		else
			index12 = index11+1;

		if (OHT1CCT[index1] == olderCuckoo)
			return 3;

		if (OHT1CCT[index11] == olderCuckoo)
			return 3;

		if (OHT1CCT[index12] == olderCuckoo)
			return 3;

		if (OHT1CCT[index1] == 0)
		{
			OHT1CCT[index1] = olderCuckoo;
			return 2;
		}

		if (OHT1CCT[index11] == 0)
		{
			OHT1CCT[index11] = olderCuckoo;
			return 2;
		}

		if (OHT1CCT[index12] == 0)
		{
			OHT1CCT[index12] = olderCuckoo;
			return 2;
		}

		aux = OHT1CCT[index1];
		OHT0CCT[index1] = olderCuckoo;
		olderCuckoo = aux;

		sprintf(str, "%d", olderCuckoo);
		index0 = HASH_OHT_CCT0;
		if (index0 == CCT_OHT_SIZE)
			index01 = 0;
		else
			index01 = index0+1;
		if (index01 == CCT_OHT_SIZE)
			index02 = 0;
		else
			index02 = index01+1;

		if (OHT0CCT[index0] == olderCuckoo)
			return 3;

		if (OHT0CCT[index01] == olderCuckoo)
			return 3;

		if (OHT0CCT[index02] == olderCuckoo)
			return 3;

		if (OHT0CCT[index0] == 0)
		{
			OHT0CCT[index0] = olderCuckoo;
			return 2;
		}

		if (OHT0CCT[index01] == 0)
		{
			OHT0CCT[index01] = olderCuckoo;
			return 2;
		}

		if (OHT0CCT[index02] == 0)
		{
			OHT0CCT[index02] = olderCuckoo;
			return 2;
		}
	}

	return 0;
}

inline int cctInsertFilter(unsigned int key, int tamOrders)
{
	char str[10];

	uint32_t olderCuckoo;
	uint32_t aux;
	uint16_t bigDesloc;
	uint16_t littleDesloc;

	uint32_t index0;
	uint32_t index1;

	//Filter Phase
	uint32_t vIndex0;
	uint32_t iIndex0;
	
	uint32_t vIndex1;
	uint32_t iIndex1;

	index0 = HASH_FILTER0;
	olderCuckoo = HASH_FINGERPRINT;
	sprintf(str, "%d", olderCuckoo);
	index1 = HASH_FILTER1;

	vIndex0 = (uint32_t) index0/5; //vector index
	iIndex0 = index0%5; //bucket index inside 32 bits word
		
	vIndex1 = (uint32_t) index1/5; //vector index
	iIndex1 = index1%5; //bucket index inside 32 bits word

	if ((((filter0CCT[vIndex0] << ((4-iIndex0)*CCT_FINGERPRINT_SIZE) >> 56)) == ((uint64_t)olderCuckoo)) ||
			(((filter1CCT[vIndex1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE) >> 56)) == ((uint64_t)olderCuckoo)))
		return 3;

	if ((((filter0CCT[vIndex0] << ((4-iIndex0)*CCT_FINGERPRINT_SIZE) >> 56)) == 0))
	{
		filter0CCT[vIndex0] = filter0CCT[vIndex0]
				| (((uint64_t)olderCuckoo) << ((iIndex0*CCT_FINGERPRINT_SIZE)+24)) ;
		return 1;
	}

	if ((((filter1CCT[vIndex1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE) >> 56)) == 0))
	{
		filter1CCT[vIndex1] = filter1CCT[vIndex1]
				| (((uint64_t)olderCuckoo) << ((iIndex1*CCT_FINGERPRINT_SIZE)+24)) ;
		return 1;
	}

	for (int j=0; j<CCT_THRESHOLD; j++)
	{
		//Always insert on the first table
		aux = (filter0CCT[vIndex0] << ((4-iIndex0)*CCT_FINGERPRINT_SIZE)) >> 56;
		bigDesloc = (4+iIndex0)*CCT_FINGERPRINT_SIZE;
		littleDesloc = (5-iIndex0)*CCT_FINGERPRINT_SIZE;
		if (iIndex0 == 4)
			filter0CCT[vIndex0] = ((filter0CCT[vIndex0] << littleDesloc) >> littleDesloc) 
				| (((uint64_t)olderCuckoo) << ((iIndex0*CCT_FINGERPRINT_SIZE)+24)) 
				| ((filter0CCT[vIndex0] >> bigDesloc) << bigDesloc);
		else
			filter0CCT[vIndex0] = ((filter0CCT[vIndex0] << littleDesloc) >> littleDesloc) 
				| (((uint64_t)olderCuckoo) << ((iIndex0*CCT_FINGERPRINT_SIZE)+24)) 
				| ((filter0CCT[vIndex0] >> bigDesloc) << bigDesloc);
		olderCuckoo = aux;

		sprintf(str, "%d", olderCuckoo);
		index1 = HASH_FILTER1;
		
		vIndex1 = (uint32_t) index1/5; //vector index
		iIndex1 = index1%5; //bucket index inside 32 bits word	

		if ((((filter1CCT[vIndex1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE) >> 56)) == olderCuckoo))
			return 3;

		if ((((filter1CCT[vIndex1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE) >> 56)) == 0))
		{
			filter1CCT[vIndex1] = filter1CCT[vIndex1]
					| (((uint64_t)olderCuckoo) << ((iIndex1*CCT_FINGERPRINT_SIZE)+24)) ;
			return 1;
		}

		aux = (filter1CCT[vIndex1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE)) >> 56;
		bigDesloc = (4+iIndex1)*CCT_FINGERPRINT_SIZE;
		littleDesloc = (5-iIndex1)*CCT_FINGERPRINT_SIZE;
		if (iIndex1 == 4)
			filter1CCT[vIndex1] = ((filter1CCT[vIndex1] << littleDesloc) >> littleDesloc) 
				| (((uint64_t)olderCuckoo) << ((iIndex1*CCT_FINGERPRINT_SIZE)+24)) 
				| ((filter1CCT[vIndex1] >> bigDesloc) << bigDesloc);
		else
			filter1CCT[vIndex1] = ((filter1CCT[vIndex1] << littleDesloc) >> littleDesloc) 
				| (((uint64_t)olderCuckoo) << ((iIndex1*CCT_FINGERPRINT_SIZE)+24)) 
				| ((filter0CCT[vIndex1] >> bigDesloc) << bigDesloc);
		olderCuckoo = aux;

		sprintf(str, "%d", olderCuckoo);
		index0 = index1;
		index0 = HASH_FILTER1;
		
		vIndex0 = (uint32_t) index0/5; //vector index
		iIndex0 = index0%5; //bucket index inside 32 bits word

		if ((((filter0CCT[vIndex0] << ((4-iIndex0)*CCT_FINGERPRINT_SIZE) >> 56)) == olderCuckoo))
			return 3;

		if ((((filter1CCT[vIndex1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE) >> 56)) == 0))
		{
			filter0CCT[vIndex0] = filter0CCT[vIndex0]
					| (((uint64_t)olderCuckoo) << ((iIndex0*CCT_FINGERPRINT_SIZE)+24)) ;
			return 1;
		}
	}
	return 2;
}

inline uint64_t popCount(uint64_t bits)
{
	uint64_t count;
	bits = bits>>24;
	for (count = 0; bits > 0; bits=bits >> 8)
		count+= (((bits<<63)>>63) |  ((bits<<62)>>63) | ((bits<<61)>>63) | ((bits<<60)>>63) |
	 			((bits<<59)>>63) | ((bits<<58)>>63) | ((bits<<57)>>63) | ((bits<<56)>>63));
	return count;
}

inline void generateCCT(column_orders * c_orders, int tamOrders)
{
	clock_t init, end;
	uint32_t dup=0;
	uint32_t dupHT=0;

	uint32_t cctOccFingerprint=0;
	uint32_t cctOHTFingerprint=0;

	uint64_t actualPopCounter0;
	uint64_t oldPopCounter0;

	uint64_t actualPopCounter1;
	uint64_t oldPopCounter1;

	uint32_t cctOHT=0;
	uint32_t cctHT=0;

	init = clock();
	
	likwid_markerStartRegion("Initialization");
	//Allocate CHT
	ctSize0 = 0;
	ctSize1 = 0;

	//Initialize OHT and bitmap
	for (int i=0; i<CCT_OHT_SIZE;i++)
	{
		OHT0CCT[i]=0;
		OHT1CCT[i]=0;
	}

	for (int i=0; i<CCT_FILTER_SIZE; i++)
	{
		filter0CCT[i]=0;
		filter1CCT[i]=0;
	}

	likwid_markerStopRegion("Initialization");
	end = clock();
	printf("Initialization: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	init = clock();
	likwid_markerStartRegion("Generation");

	for (int i=0; i<tamOrders; i++)
	{
		switch (cctInsertFilter(c_orders[i].O_CUSTKEY, tamOrders))
		{
			case 0:
				printf("ERROR: Something went wrong while inserting the key %u on the CHT\n", c_orders[i].O_CUSTKEY);
				break;
			case 1:
				cctOccFingerprint++;
				break;
			case 2:
				cctOHTFingerprint++;
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
		oldPopCounter0 = popCount(filter0CCT[i]);
		actualPopCounter0 = actualPopCounter0 + oldPopCounter0;
		filter0CCT[i] = ((filter0CCT[i]>>24)<<24) | actualPopCounter0;

		oldPopCounter1 = popCount(filter1CCT[i]);
		actualPopCounter1 = actualPopCounter1 + oldPopCounter1;
		filter1CCT[i] = ((filter1CCT[i]>>24)<<24) | actualPopCounter1;
	}

	ctSize0 = actualPopCounter0;
	ctSize1 = actualPopCounter1;

	cuckooTable0 = malloc(sizeof(uint32_t)*ctSize0);
	cuckooTable1 = malloc(sizeof(uint32_t)*ctSize1);

	for (int i=0; i<ctSize0; i++)
		cuckooTable0[i] = 0;

	for (int i=0; i<ctSize1; i++)
		cuckooTable1[i] = 0;

	for (int i=0; i<tamOrders; i++)
	{
		switch (cctInsertConciseTable(c_orders[i].O_CUSTKEY, tamOrders))
		{
			case 0:
				printf("ERROR: Something went wrong while inserting the key %u on the CHT\n", c_orders[i].O_CUSTKEY);
				break;
			case 1:
				cctHT++;
				break;
			case 2:
				cctOHT++;
				break;
			case 3:
				dupHT++;
				break;
		}
	}

	likwid_markerStopRegion("Generation");
	end = clock();
	printf("CHT Generation: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
	printf("OHT has %d occupied buckets and %d on the filter \n", cctOHT, cctOHTFingerprint);
	printf("CHT has %d occupied buckets and %d on the filter \n", cctHT, cctOccFingerprint);
}

inline int cctLookUp(uint32_t key)
{
	char str[10];
	sprintf(str, "%d", key);

	uint32_t fingerPrint;

	uint32_t index0, index01, index02;
	uint32_t index1, index11, index12;

	//Filter Phase
	uint32_t vIndex0;
	uint32_t iIndex0;
	uint32_t tIndex0;
	
	uint32_t vIndex1;
	uint32_t iIndex1;
	uint32_t tIndex1;

	index0 = HASH_FILTER0;
	fingerPrint = HASH_FINGERPRINT;
	sprintf(str, "%d", fingerPrint);
	index1 = HASH_FILTER1;

	vIndex0 = (uint32_t) index0/5; //vector index
	iIndex0 = index0%5; //bucket index inside 32 bits word
		
	vIndex1 = (uint32_t) index1/5; //vector index
	iIndex1 = index1%5; //bucket index inside 32 bits word

	if ((((filter0CCT[vIndex0] << ((4-iIndex0)*CCT_FINGERPRINT_SIZE) >> 56)) == ((uint64_t)fingerPrint)) ||
		(((filter1CCT[vIndex1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE) >> 56)) == ((uint64_t)fingerPrint)))
	{
		tIndex0 = ((filter0CCT[vIndex0]<<40)>>40) - (popCount((filter0CCT[vIndex0]<<((5-iIndex0)*CCT_FINGERPRINT_SIZE))>>((5-iIndex0)*CCT_FINGERPRINT_SIZE)));
		tIndex1 = ((filter1CCT[vIndex1]<<40)>>40) - (popCount((filter1CCT[vIndex1]<<((5-iIndex1)*CCT_FINGERPRINT_SIZE))>>((5-iIndex1)*CCT_FINGERPRINT_SIZE)));

		if ((cuckooTable0[tIndex0] == key) || (cuckooTable1[tIndex1] == key))
			return 1;
	}

	//Search OHT
	sprintf(str, "%d", key);
	index0 = HASH_OHT_CCT0;
	if (index0+1 >= CCT_OHT_SIZE)
		index01 = 0;
	else
		index01 = index0+1;
	if (index01+1 >= CCT_OHT_SIZE)
		index02 = 0;
	else
		index02 = index01+1;

	index1 = HASH_OHT_CCT1;
	if (index1+1 >= CCT_OHT_SIZE)
		index11 = 0;
	else
		index11 = index1+1;
	if (index11+1 >= CCT_OHT_SIZE)
		index12 = 0;
	else
		index12 = index11+1;

	if (OHT0CCT[index0] == key || OHT1CCT[index1] == key || 
		OHT0CCT[index01] == key || OHT1CCT[index11] == key ||
		OHT0CCT[index02] == key || OHT1CCT[index12] == key)
			return 1;

	return 0;
}

inline int cctJoin(column_customer * c_customer, column_orders * c_orders, int tamCustomer, int tamOrders)
{
	clock_t init, end;
	uint32_t index=0;

	generateCCT(c_orders, tamOrders);

	init=clock();
	likwid_markerStartRegion("Core");
	for (int i=0; i<tamCustomer; i++)
	{
		if (cctLookUp(c_customer[i].C_CUSTKEY) == 0)
			index++;
	}
	likwid_markerStopRegion("Core");
	end=clock();

	printf("Join core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	return index;
}