#include "../hash/hash.h"
#include "../load.h"


#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define HASH_BUCKETS 131072

#define HASH_MURMUR2 MurmurHash2(str, strlen(str), tamCustomer) & (nBuckets-1)
#define HASH_JENKINS hash_jenkins(str) & (nBuckets-1)
#define HASH_FNV1A fnv1a(key) & (nBuckets-1)
#define HASH_MURMUR3 murmurhash3(str, strlen(str), tamCustomer) & (nBuckets-1)
#define HASH_ANY hash_any(str, strlen(str)) & (nBuckets-1)

int main(int argc, char ** argv)
{
	int nBuckets;
	int tamCustomer=150000;
	char str[10];
	FILE * fAny, * fJenkins, * fMurmur3, * fMurmur2, * fFnv1a;
	clock_t init, end;
	unsigned int key;

	int vAny[tamCustomer], vJenkins[tamCustomer], vMurmur3[tamCustomer], vMurmur2[tamCustomer], vFnv1a[tamCustomer];

	//HashAny
	fAny = fopen("/home/flav/Mestrado/MHaJoL/hashTest/fAny.csv", "w+");
	nBuckets = HASH_BUCKETS;
	init = clock();
	for (int i=0; i<tamCustomer; i++)
	{
		sprintf(str, "%d", i);
		vAny[i] = HASH_ANY;
	}
	end = clock();

	for (int i=0; i<tamCustomer; i++)
		fprintf(fAny, "%d;%d\n", i, vAny[i]);

	printf("Hash Any %.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	//Jenkins
	fJenkins = fopen("/home/flav/Mestrado/MHaJoL/hashTest/fJenkins.csv", "w+");
	init = clock();
	for (int i=0; i<tamCustomer; i++)
	{
		sprintf(str, "%d", i);
		vJenkins[i] = HASH_JENKINS;
	}
	end = clock();

	for (int i=0; i<tamCustomer; i++)
		fprintf(fJenkins, "%d;%d\n", i,vJenkins[i]);

	printf("Jenkins %.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	//Murmur3
	fMurmur3 = fopen("/home/flav/Mestrado/MHaJoL/hashTest/fMurmur3.csv", "w+");
	init = clock();
	for (int i=0; i<tamCustomer; i++)
	{
		sprintf(str, "%d", i);
		vMurmur3[i] = HASH_MURMUR3;
	}
	end = clock();

	for (int i=0; i<tamCustomer; i++)
		fprintf(fMurmur3, "%d;%d\n", i,vMurmur3[i]);

	printf("Murmur3 %.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	//Murmur2
	fMurmur2 = fopen("/home/flav/Mestrado/MHaJoL/hashTest/fMurmur2.csv", "w+");
	init = clock();
	for (int i=0; i<tamCustomer; i++)
	{
		sprintf(str, "%d", i);
		vMurmur2[i] = HASH_MURMUR2;
	}
	end = clock();

	for (int i=0; i<tamCustomer; i++)
		fprintf(fMurmur2, "%d;%d\n", i,vMurmur2[i]);

	printf("Murmur2 %.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	//FNV1a
	fFnv1a = fopen("/home/flav/Mestrado/MHaJoL/hashTest/fFnv1a.csv", "w+");
	init = clock();
	for (int i=0; i<tamCustomer; i++)
	{
		key = i;
		vFnv1a[i] = HASH_FNV1A;
	}
	end = clock();

	for (int i=0; i<tamCustomer; i++)
		fprintf(fFnv1a, "%d;%d\n", i,vFnv1a[i]);

	printf("FNV1a %.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));
}
