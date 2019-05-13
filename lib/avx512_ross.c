#include "ViViD.h"

/* Copyright (c) 2015
 * The Trustees of Columbia University in the City of New York
 * All rights reserved.
 *
 * Author:  Orestis Polychroniou  (orestis@cs.columbia.edu)
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * 3. Neither the name of the copyright holder nor the names of its
 *    contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR
 * TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#ifndef _GNU_SOURCE
#define _GNU_SOURCE
#endif

inline int avx512Join(column_customer * c_customer, column_orders * c_orders)
{
	uint32_t index=0;
	uint32_t factor[2];
	rand32_t *gen = rand32_init(time());


	for (int i=0; i<REAL_TAB_SIZE; i++)
		cuckoo[i]=0;

	generate_factors(factor, gen);
	avx512Generate(c_orders, factor);

    return index; 
}

inline int avx512Generate(column_orders * c_orders,  uint32_t factor[2])
{
	clock_t init, end;
	int key[tamOrders];
	inr rids[tamOrders];
	for (unsigned int i=0; i<tamOrders;i++)
	{
		keys[i] = c_orders[i].O_CUSTKEY;
		rids[i] = c_orders[i].O_CUSTKEY;
	}
	size_t size = tamOrders;
	uint32_t empty = 0xFFFFFFFF;

	// generate masks
	__m512i mask_pack = _mm512_set_epi32(15, 7, 14, 6, 13, 5, 12, 4, 11, 3, 10, 2, 9, 1, 8, 0);
	__m512i mask_unpack = _mm512_set_epi32(15, 13, 11, 9, 7, 5, 3, 1, 14, 12, 10, 8, 6, 4, 2, 0);
	__m512i mask_factor_1 = _mm512_set1_epi32(factor[0]);
	__m512i mask_factor_2 = _mm512_set1_epi32(factor[1]);
	__m512i mask_buckets = _mm512_set1_epi32(REAL_TAB_SIZE-1);
	__m512i mask_empty = _mm512_set1_epi32(empty);
	__mmask16 blend_0000 = _mm512_int2mask(0x0000);
	__mmask16 blend_AAAA = _mm512_int2mask(0xAAAA);
	__mmask16 blend_5555 = _mm512_int2mask(0x5555);
	// reset table
	__m512i mask_half_empty = _mm512_set1_epi64(empty);
	size_t i = REAL_TAB_SIZE-1;
	assert(table == align(cuckoo));
	while (i & 7)
		table[--i] = empty;
	while (i) {
		i -= 8;
		_mm512_store_epi32(&cuckoo[i], mask_half_empty);
	}

	// main building loop
	__m512i hash, rid, key = mask_empty;
	size_t size_minus_16 = size - 16;
	init=clock();
	if (size >= 16) do {
		// replace invalid items with new items from input
		__mmask16 k = _mm512_cmpeq_epi32_mask(key, mask_empty);
		key = _mm512_mask_loadunpacklo_epi32(key, k, &keys[i]);
		key = _mm512_mask_loadunpackhi_epi32(key, k, &keys[i + 16]);
		rid = _mm512_mask_loadunpacklo_epi32(rid, k, &rids[i]);
		rid = _mm512_mask_loadunpackhi_epi32(rid, k, &rids[i + 16]);
		i += _mm_countbits_64(_mm512_kconcatlo_64(blend_0000, k));

		// compute both hash functions
		__m512i hash_1 = _mm512_mullo_epi32(key, mask_factor_1);
		__m512i hash_2 = _mm512_mullo_epi32(key, mask_factor_2);
		hash_1 = _mm512_mulhi_epu32(hash_1, mask_buckets);
		hash_2 = _mm512_mulhi_epu32(hash_2, mask_buckets);

		// pick alternate function from the one used before
		__m512i hash_12 = _mm512_add_epi32(hash_1, hash_2);
		hash = _mm512_sub_epi32(hash_12, hash);

		// new items try first hash
		hash = _mm512_mask_blend_epi32(k, hash, hash_1);

		// access first choice buckets
		__m512i lo = _mm512_i32logather_epi64(hash, table, 8);
		__m512i hash_reverse = _mm512_permute4f128_epi32(hash, _MM_PERM_BADC);
		__m512i hi = _mm512_i32logather_epi64(hash_reverse, table, 8);

		// unpack key and check for non-empty if new
		__m512i tab = _mm512_mask_blend_epi32(blend_AAAA, lo, _mm512_swizzle_epi32(hi, _MM_SWIZ_REG_CDAB));
		tab = _mm512_permutevar_epi32(mask_unpack, tab);
		k = _mm512_mask_cmpneq_epi32_mask(k, tab, mask_empty);

		// new items try second hash if not empty
		hash = _mm512_mask_blend_epi32(k, hash, hash_2);

		// new items access second choice buckets if first choice was not empty
		lo = _mm512_mask_i32logather_epi64(lo, k, hash, table, 8);
		hash_reverse = _mm512_permute4f128_epi32(hash, _MM_PERM_BADC);
		k = _mm512_kmerge2l1h(k, k);
		hi = _mm512_mask_i32logather_epi64(hi, k, hash_reverse, table, 8);

		// unpack key and payload
		__m512i tab_key = _mm512_mask_blend_epi32(blend_AAAA, lo, _mm512_swizzle_epi32(hi, _MM_SWIZ_REG_CDAB));
		__m512i tab_rid = _mm512_mask_blend_epi32(blend_5555, hi, _mm512_swizzle_epi32(lo, _MM_SWIZ_REG_CDAB));
		tab_key = _mm512_permutevar_epi32(mask_unpack, tab_key);
		tab_rid = _mm512_permutevar_epi32(mask_unpack, tab_rid);

		// pack input keys and payloads
		__m512i key_tmp = _mm512_permutevar_epi32(mask_pack, key);
		__m512i rid_tmp = _mm512_permutevar_epi32(mask_pack, rid);
		lo = _mm512_mask_blend_epi32(blend_AAAA, key_tmp, _mm512_swizzle_epi32(rid_tmp, _MM_SWIZ_REG_CDAB));
		hi = _mm512_mask_blend_epi32(blend_5555, rid_tmp, _mm512_swizzle_epi32(key_tmp, _MM_SWIZ_REG_CDAB));

		// scatter input keys and payloads
		_mm512_i32loscatter_epi64(table, hash, lo, 8);
		_mm512_i32loscatter_epi64(table, hash_reverse, hi, 8);

		// detect conflicts
		__m512i key_back = _mm512_i32gather_epi32(hash, table, 8);
		k = _mm512_cmpeq_epi32_mask(key, key_back);

		// overwrite non-conflicting items from table
		key = _mm512_mask_blend_epi32(k, key, tab_key);
		rid = _mm512_mask_blend_epi32(k, rid, tab_rid);
	} while (i <= size_minus_16);

	// store last items in stack
	uint32_t tmp_key[32];
	uint32_t tmp_rid[32];
	__mmask16 k = _mm512_cmpneq_epi32_mask(key, mask_empty);
	_mm512_mask_packstorelo_epi32(&tmp_key[0],  k, key);
	_mm512_mask_packstorehi_epi32(&tmp_key[16], k, key);
	_mm512_mask_packstorelo_epi32(&tmp_rid[0],  k, rid);
	_mm512_mask_packstorehi_epi32(&tmp_rid[16], k, rid);
	size_t c = _mm_countbits_64(_mm512_kconcatlo_64(blend_0000, k));
	for (; i != size ; ++i, ++c) {
		tmp_key[c] = keys[i];
		tmp_rid[c] = rids[i];
	}
	// use scalar code to insert last items
	uint32_t factor_1 = factor[0];
	uint32_t factor_2 = factor[1];
	for (i = 0 ; i != c ; ++i) {
		uint32_t k = tmp_key[i];
		uint64_t p = tmp_rid[i];
		p = (p << 32) | k;
		size_t h = (uint32_t) (k * factor_1);
		h = (h * buckets) >> 32;
		if (empty == (uint32_t) table[h]) {
			table[h] = p;
			continue;
		}
		h = (uint32_t) (k * factor_2);
		h = (h * buckets) >> 32;
		uint64_t t = table[h];
		if (empty == (uint32_t) t) {
			table[h] = p;
			continue;
		}
		uint64_t loops = 0;
		table[h] = p;
		do {
			if (++loops == buckets) return 0;
			size_t h1 = (uint32_t) (factor_1 * (uint32_t) t);
			size_t h2 = (uint32_t) (factor_2 * (uint32_t) t);
			h1 = (h1 * buckets) >> 32;
			h2 = (h2 * buckets) >> 32;
			h = h1 + h2 - h;
			p = t;
			t = table[h];
			table[h] = p;
		} while (empty != (uint32_t) t);
	}
	end = clock();
	printf("Generation %.f ms \n\n", ((double)(end - init) / (CLOCKS_PER_SEC / 1000)));

	return 1;
}

inline void generate_factors(uint32_t factor[2], rand32_t *gen)
{
	uint32_t f1, f2;
	do {
		f1 = rand32_next(gen) | 1;
		f2 = rand32_next(gen) | 1;
	} while (((f1 - f2) & 3) == 0);
	factor[0] = f1;
	factor[1] = f2;
}

inline rand64_t *rand64_init(uint64_t seed)
{
	rand64_t *state = malloc(sizeof(rand64_t));
	uint64_t *n = state->num;
	size_t i;
	n[0] = seed;
	for (i = 0 ; i != 311 ; ++i)
		n[i + 1] = 6364136223846793005ull *
		           (n[i] ^ (n[i] >> 62)) + i + 1;
	state->index = 312;
	return state;
}

inline uint64_t rand64_next(rand64_t *state)
{
	uint64_t x, *n = state->num;
	if (state->index == 312) {
		size_t i = 0;
		do {
			x = n[i] & 0xffffffff80000000ull;
			x |= n[i + 1] & 0x7fffffffull;
			n[i] = n[i + 156] ^ (x >> 1);
			n[i] ^= 0xb5026f5aa96619e9ull & -(x & 1);
		} while (++i != 156);
		n[312] = n[0];
		do {
			x = n[i] & 0xffffffff80000000ull;
			x |= n[i + 1] & 0x7fffffffull;
			n[i] = n[i - 156] ^ (x >> 1);
			n[i] ^= 0xb5026f5aa96619e9ull & -(x & 1);
		} while (++i != 312);
		state->index = 0;
	}
	x = n[state->index++];
	x ^= (x >> 29) & 0x5555555555555555ull;
	x ^= (x << 17) & 0x71d67fffeda60000ull;
	x ^= (x << 37) & 0xfff7eee000000000ull;
	x ^= (x >> 43);
	return x;
}

rand32_t *rand32_init(uint32_t seed)
{
	rand32_t *state = malloc(sizeof(rand32_t));
	uint32_t *n = state->num;
	size_t i;
	n[0] = seed;
	for (i = 0 ; i != 623 ; ++i)
		n[i + 1] = 0x6c078965 * (n[i] ^ (n[i] >> 30));
	state->index = 624;
	return state;
}

uint32_t rand32_next(rand32_t *state)
{
	uint32_t y, *n = state->num;
	if (state->index == 624) {
		size_t i = 0;
		do {
			y = n[i] & 0x80000000;
			y += n[i + 1] & 0x7fffffff;
			n[i] = n[i + 397] ^ (y >> 1);
			n[i] ^= 0x9908b0df & -(y & 1);
		} while (++i != 227);
		n[624] = n[0];
		do {
			y = n[i] & 0x80000000;
			y += n[i + 1] & 0x7fffffff;
			n[i] = n[i - 227] ^ (y >> 1);
			n[i] ^= 0x9908b0df & -(y & 1);
		} while (++i != 624);
		state->index = 0;
	}
	y = n[state->index++];
	y ^= (y >> 11);
	y ^= (y << 7) & 0x9d2c5680;
	y ^= (y << 15) & 0xefc60000;
	y ^= (y >> 18);
	return y;
}

void *mamalloc(size_t size)
{
	void *ptr = NULL;
	return posix_memalign(&ptr, 64, size) ? NULL : ptr;
}

void *align(const void *p)
{
	size_t i = 63 & (size_t) p;
	return (void*) (i ? p + 64 - i : p);
}