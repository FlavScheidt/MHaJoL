#include "join.h"
//                                           QUERY PLAN                                           
// -----------------------------------------------------------------------------------------------
//  Merge Anti Join  (cost=1098955.75..1275080.65 rows=750054 width=6)
//    Merge Cond: (customer.c_custkey = orders.o_custkey)
//    ->  Index Scan using customer_pkey on customer  (cost=0.43..75887.06 rows=1500108 width=10)
//    ->  Materialize  (cost=1098955.32..1126523.32 rows=5513600 width=8)
//          ->  Sort  (cost=1098955.32..1112739.32 rows=5513600 width=8)
//                Sort Key: orders.o_custkey
//                ->  Seq Scan on orders  (cost=0.00..330816.00 rows=5513600 width=8)
// (7 rows)

int nestedLoopJoin (column_customer *c_customer, column_orders *c_orders, int tamCustomer, int tamOrders, float * t_result)
{
	int exists = 0;
	int index = 0;

	for (int i=0; i<tamCustomer; i++)
	{
		for (int j=0; j<tamOrders && exists == 0; j++)
		{
			if (c_customer[i].C_CUSTKEY == c_orders[j].O_CUSTKEY)
				exists = 1;
		}
		if (!exists)
		{
			t_result[index] = c_customer[i].C_ACCTBAL;
			index++;
		}
		else
			exists = 0;
	}
	return index;
}
                                                              // QUERY PLAN                                                              
// --------------------------------------------------------------------------------------------------------------------------------------
//  Hash Anti Join  (cost=738976.61..1065154.68 rows=789930 width=6) (actual time=20322.447..20501.759 rows=50004 loops=1)
//    Hash Cond: (customer.c_custkey = orders.o_custkey)
//    ->  Seq Scan on customer  (cost=0.00..57119.33 rows=1650233 width=10) (actual time=92.703..128.634 rows=150000 loops=1)
//    ->  Hash  (cost=468259.27..468259.27 rows=16500827 width=8) (actual time=20226.815..20226.815 rows=1500000 loops=1)
//          Buckets: 131072  Batches: 256  Memory Usage: 1269kB
//          ->  Seq Scan on orders  (cost=0.00..468259.27 rows=16500827 width=8) (actual time=16228.618..16531.542 rows=1500000 loops=1)
//  Planning time: 19.752 ms
//  Execution time: 20503.874 ms
// (8 rows)


int hashJoin (column_customer *c_customer, column_orders *c_orders, int tamCustomer, int tamOrders, float * t_result)
{
	//int exists = 0;
	int index = 0;
	int nResult=0;
	int nBuckets = HASH_BUCKETS;
	clock_t init, end;

	linkedList ** buckets;
	buckets = malloc(nBuckets*sizeof(linkedList));

	init = clock();
	//Initialize buckets
	for (int n = 0; n<nBuckets; n++)
	{
		buckets[n] = (linkedList *) malloc (sizeof(linkedList));
		buckets[n]->C_CUSTKEY = -1;
		buckets[n]->next = NULL;
	}
	end = clock();

	printf("Initialization: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	generateHashTable(c_orders, tamOrders, buckets);

	init = clock();
	//Loop on orders to verify the existence of the register
	for (int i=0; i<tamCustomer; i++)
	{
		if (lookupHashTable(buckets, c_customer[i].C_CUSTKEY) == 0)
		{
			t_result[index] = c_customer[i].C_ACCTBAL;
			nResult++;
		}
	}
	end = clock();
	printf("Join Core: %.f ms \n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	return nResult;
}