#include "vecCuckooFilter.h"

// inline int vecInsertFilter(unsigned int key, int tamOrders)
// {
// 	char str[10];

// 	uint32_t olderCuckoo;
// 	uint32_t aux;
// 	uint16_t bigDesloc;
// 	uint16_t littleDesloc;

// 	uint32_t index0;
// 	uint32_t index1;

// 	//Filter Phase
// 	uint32_t vIndex0;
// 	uint32_t iIndex0;
	
// 	uint32_t vIndex1;
// 	uint32_t iIndex1;

// 	index0 = HASH_FILTER0;
// 	olderCuckoo = HASH_FINGERPRINT;
// 	sprintf(str, "%d", olderCuckoo);
// 	index1 = HASH_FILTER1;

// 	vIndex0 = (uint32_t) index0/5; //vector index
// 	iIndex0 = index0%5; //bucket index inside 32 bits word
		
// 	vIndex1 = (uint32_t) index1/5; //vector index
// 	iIndex1 = index1%5; //bucket index inside 32 bits word

// 	if ((((filter0CCT[vIndex0] << ((4-iIndex0)*CCT_FINGERPRINT_SIZE) >> 56)) == ((uint64_t)olderCuckoo)) ||
// 			(((filter1CCT[vIndex1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE) >> 56)) == ((uint64_t)olderCuckoo)))
// 		return 3;

// 	if ((((filter0CCT[vIndex0] << ((4-iIndex0)*CCT_FINGERPRINT_SIZE) >> 56)) == 0))
// 	{
// 		filter0CCT[vIndex0] = filter0CCT[vIndex0]
// 				| (((uint64_t)olderCuckoo) << ((iIndex0*CCT_FINGERPRINT_SIZE)+24)) ;
// 		return 1;
// 	}

// 	if ((((filter1CCT[vIndex1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE) >> 56)) == 0))
// 	{
// 		filter1CCT[vIndex1] = filter1CCT[vIndex1]
// 				| (((uint64_t)olderCuckoo) << ((iIndex1*CCT_FINGERPRINT_SIZE)+24)) ;
// 		return 1;
// 	}

// 	for (int j=0; j<CCT_THRESHOLD; j++)
// 	{
// 		//Always insert on the first table
// 		aux = (filter0CCT[vIndex0] << ((4-iIndex0)*CCT_FINGERPRINT_SIZE)) >> 56;
// 		bigDesloc = (4+iIndex0)*CCT_FINGERPRINT_SIZE;
// 		littleDesloc = (5-iIndex0)*CCT_FINGERPRINT_SIZE;
// 		if (iIndex0 == 4)
// 			filter0CCT[vIndex0] = ((filter0CCT[vIndex0] << littleDesloc) >> littleDesloc) 
// 				| (((uint64_t)olderCuckoo) << ((iIndex0*CCT_FINGERPRINT_SIZE)+24)) 
// 				| ((filter0CCT[vIndex0] >> bigDesloc) << bigDesloc);
// 		else
// 			filter0CCT[vIndex0] = ((filter0CCT[vIndex0] << littleDesloc) >> littleDesloc) 
// 				| (((uint64_t)olderCuckoo) << ((iIndex0*CCT_FINGERPRINT_SIZE)+24)) 
// 				| ((filter0CCT[vIndex0] >> bigDesloc) << bigDesloc);
// 		olderCuckoo = aux;

// 		sprintf(str, "%d", olderCuckoo);
// 		index1 = HASH_FILTER1;
		
// 		vIndex1 = (uint32_t) index1/5; //vector index
// 		iIndex1 = index1%5; //bucket index inside 32 bits word	

// 		if ((((filter1CCT[vIndex1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE) >> 56)) == olderCuckoo))
// 			return 3;

// 		if ((((filter1CCT[vIndex1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE) >> 56)) == 0))
// 		{
// 			filter1CCT[vIndex1] = filter1CCT[vIndex1]
// 					| (((uint64_t)olderCuckoo) << ((iIndex1*CCT_FINGERPRINT_SIZE)+24)) ;
// 			return 1;
// 		}

// 		aux = (filter1CCT[vIndex1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE)) >> 56;
// 		bigDesloc = (4+iIndex1)*CCT_FINGERPRINT_SIZE;
// 		littleDesloc = (5-iIndex1)*CCT_FINGERPRINT_SIZE;
// 		if (iIndex1 == 4)
// 			filter1CCT[vIndex1] = ((filter1CCT[vIndex1] << littleDesloc) >> littleDesloc) 
// 				| (((uint64_t)olderCuckoo) << ((iIndex1*CCT_FINGERPRINT_SIZE)+24)) 
// 				| ((filter1CCT[vIndex1] >> bigDesloc) << bigDesloc);
// 		else
// 			filter1CCT[vIndex1] = ((filter1CCT[vIndex1] << littleDesloc) >> littleDesloc) 
// 				| (((uint64_t)olderCuckoo) << ((iIndex1*CCT_FINGERPRINT_SIZE)+24)) 
// 				| ((filter0CCT[vIndex1] >> bigDesloc) << bigDesloc);
// 		olderCuckoo = aux;

// 		sprintf(str, "%d", olderCuckoo);
// 		index0 = index1;
// 		index0 = HASH_FILTER1;
		
// 		vIndex0 = (uint32_t) index0/5; //vector index
// 		iIndex0 = index0%5; //bucket index inside 32 bits word

// 		if ((((filter0CCT[vIndex0] << ((4-iIndex0)*CCT_FINGERPRINT_SIZE) >> 56)) == olderCuckoo))
// 			return 3;

// 		if ((((filter1CCT[vIndex1] << ((4-iIndex1)*CCT_FINGERPRINT_SIZE) >> 56)) == 0))
// 		{
// 			filter0CCT[vIndex0] = filter0CCT[vIndex0]
// 					| (((uint64_t)olderCuckoo) << ((iIndex0*CCT_FINGERPRINT_SIZE)+24)) ;
// 			return 1;
// 		}
// 	}
// 	return 2;
// }