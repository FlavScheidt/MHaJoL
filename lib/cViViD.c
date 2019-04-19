#include "cViViD.h"

inline void cViViDGenerate(column_orders * c_orders)
{
	cViViDGenerateFilter(c_orders);
	return;
}

inline int cViViDJoin(column_customer * c_customer, column_orders * c_orders)
{
	clock_t init, end;
	uint32_t index=0;
	int customer[tamCustomer];

	__m256i keys;

	__m256i mask_1 = _mm256_set1_epi32(0);
	__m256i mask_0 = _mm256_set1_epi32(0);
	mask_1 = _mm256_cmpeq_epi32(mask_1, mask_0);

	for (int i=0; i<REAL_TAB_SIZE; i++)
		cuckoo[i]=0;

	cViViDGenerate(c_orders);

	// for (int i=0; i<REAL_TAB_SIZE; i++)
	// {
	// 	if (cuckoo[i] != 0)
	// 	{
	// 		OCC++;
	// 		// printf("%d\n", cuckoo[i]);
	// 	}
	// }

	// for (unsigned int i=0; i<tamCustomer;i++)
	// 	customer[i] = c_customer[i].C_CUSTKEY;

	// init=clock();
	// likwid_markerStartRegion("Core");
	// for (unsigned int i=0; i<tamCustomer; i=i+8)
	// {
	// 	keys = _mm256_maskload_epi32(&customer[i], mask_1);
	// 	index += vecCuckooLookUp(keys);
	// }
	// likwid_markerStopRegion("Core");
	// end=clock();

	// printf("Join core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
	// printf("Cuckoo Occupation %d\n", OCC);

    return index; 
}
