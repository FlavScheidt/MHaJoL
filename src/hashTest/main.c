#include "../../lib/hash/hash.h"
// #include "../load.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#ifndef __cplusplus
#include <stdalign.h>   // C11 defines _Alignas().  This header defines alignas()
#endif

#define HASH_BUCKETS 65536

#define HASH_MURMUR2 MurmurHash2(str, strlen(str), tamCustomer) & (nBuckets-1)
#define HASH_JENKINS hash_jenkins(str) & (nBuckets-1)
#define HASH_FNV1A fnv1a(key) & (nBuckets-1)
#define HASH_MURMUR3 murmurhash3(str, strlen(str), tamCustomer) & (nBuckets-1)
#define HASH_ANY hash_any(str, strlen(str)) & (nBuckets-1)

int main(int argc, char ** argv)
{
	int nBuckets;
	int tamCustomer=150000;
	// char str[10];
	FILE /* * fAny, * fJenkins, * fMurmur3, * fMurmur2, * fFnv1a, * */ * fMurmur3SIMD, * fFnv1aSIMD;
	clock_t init, end;
	// unsigned int key;

	alignas(32) uint32_t /*vAny[tamCustomer], vJenkins[tamCustomer], vMurmur3[tamCustomer], vMurmur2[tamCustomer], vFnv1a[tamCustomer],*/ vMurmurSIMD[tamCustomer], vFnv1aSIMD[tamCustomer];

	// vFnv1aSIMD = malloc(sizeof(uint32_t)*tamCustomer);

	// //HashAny
	// // fAny = fopen("/home/flav/Mestrado/MHaJoL/hashTest/fAny.csv", "w+");
	nBuckets = HASH_BUCKETS-1;
	// init = clock();
	// for (int i=0; i<tamCustomer; i++)
	// {
	// 	sprintf(str, "%d", i);
	// 	vAny[i] = HASH_ANY;
	// }
	// end = clock();

	// // for (int i=0; i<tamCustomer; i++)
	// // 	fprintf(fAny, "%d;%d\n", i, vAny[i]);

	// printf("Hash Any %.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	// //Jenkins
	// // fJenkins = fopen("/home/flav/Mestrado/MHaJoL/hashTest/fJenkins.csv", "w+");
	// init = clock();
	// for (int i=0; i<tamCustomer; i++)
	// {
	// 	sprintf(str, "%d", i);
	// 	vJenkins[i] = HASH_JENKINS;
	// }
	// end = clock();

	// // for (int i=0; i<tamCustomer; i++)
	// // 	fprintf(fJenkins, "%d;%d\n", i,vJenkins[i]);

	// printf("Jenkins %.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	// //Murmur3
	// // fMurmur3 = fopen("/home/flav/Mestrado/MHaJoL/hashTest/fMurmur3.csv", "w+");
	// init = clock();
	// for (int i=0; i<tamCustomer; i++)
	// {
	// 	sprintf(str, "%d", i);
	// 	vMurmur3[i] = HASH_MURMUR3;
	// }
	// end = clock();

	// // for (int i=0; i<tamCustomer; i++)
	// // 	fprintf(fMurmur3, "%d;%d\n", i,vMurmur3[i]);

	// printf("Murmur3 %.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	// //Murmur2
	// // fMurmur2 = fopen("/home/flav/Mestrado/MHaJoL/hashTest/fMurmur2.csv", "w+");
	// init = clock();
	// for (int i=0; i<tamCustomer; i++)
	// {
	// 	sprintf(str, "%d", i);
	// 	vMurmur2[i] = HASH_MURMUR2;
	// }
	// end = clock();

	// // for (int i=0; i<tamCustomer; i++)
	// // 	fprintf(fMurmur2, "%d;%d\n", i,vMurmur2[i]);

	// printf("Murmur2 %.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	// //FNV1a
	// // fFnv1a = fopen("/home/flav/Mestrado/MHaJoL/hashTest/fFnv1a.csv", "w+");
	// init = clock();
	// for (int i=0; i<tamCustomer; i++)
	// {
	// 	key = i;
	// 	vFnv1a[i] = HASH_FNV1A;
	// }
	// end = clock();

	// // for (int i=0; i<tamCustomer; i++)
	// // 	fprintf(fFnv1a, "%d;%d\n", i,vFnv1a[i]);

	// printf("FNV1a %.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	__m256i keys;
	__m256i hashed;
	__m256i tableSizeVector = _mm256_set1_epi32(nBuckets);

	__m256i * ptr;

	//FNV1a SIMD
	fFnv1aSIMD = fopen("/home/flav/Mestrado/MHaJoL/src/hashTest/fFnv1aSIMD.csv", "w+");
	init = clock();
	for (int i=1; i<=tamCustomer; i+=8)
	{
		keys = _mm256_set_epi32(i+7, i+6, i+5, i+4, i+3, i+2, i+1, i);
		hashed = _mm256_fnv1a_epi32(keys);
		hashed = _mm256_and_si256(hashed, tableSizeVector);
		ptr = (__m256i*) (vFnv1aSIMD+(i-1));

	    _mm256_storeu_si256(ptr, hashed);
	}
	end = clock();

	for (int i=0; i<tamCustomer; i++)
		fprintf(fFnv1aSIMD, "%u;%u\n", i,vFnv1aSIMD[i]);

	printf("FNV1a SIMD %.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	fMurmur3SIMD = fopen("/home/flav/Mestrado/MHaJoL/src/hashTest/fMurmur3SIMD.csv", "w+");
	init = clock();
	for (int i=1; i<=tamCustomer; i+=8)
	{
		keys = _mm256_set_epi32(i+7, i+6, i+5, i+4, i+3, i+2, i+1, i);
		hashed = _mm256_murmur3_epi32(keys, 42);
		hashed = _mm256_and_si256(hashed, tableSizeVector);

		ptr = (__m256i*) (vMurmurSIMD+(i-1));

	    _mm256_storeu_si256(ptr, hashed);
	}
	end = clock();

	for (int i=0; i<tamCustomer; i++)
		fprintf(fMurmur3SIMD, "%u;%u\n", i,vMurmurSIMD[i]);

	printf("FNV1a %.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
}
