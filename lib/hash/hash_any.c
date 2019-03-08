#include "hash.h"

inline int hash_any(char *k, int keylen)
{
	 int a,
				b,
				c,
				len;

	/* Set up the internal state */
	len = keylen;
	a = b = c = 0x9e3779b9 + len + 3923095;

	/* If the source pointer is word-aligned, we use word-wide fetches */
	if (((uintptr_t) k & UINT32_ALIGN_MASK) == 0)
	{
		/* Code path for aligned source data */
		 const int *ka = (const int *) k;

		/* handle most of the key */
		while (len >= 12)
		{
			a += ka[0];
			b += ka[1];
			c += ka[2];
			mix(a, b, c);
			ka += 3;
			len -= 12;
		}

		/* handle the last 11 bytes */
		k = (char *) ka;
#ifdef WORDS_BIGENDIAN
		switch (len)
		{
			case 11:
				c += ((int) k[10] << 8);
				/* fall through */
			case 10:
				c += ((int) k[9] << 16);
				/* fall through */
			case 9:
				c += ((int) k[8] << 24);
				/* the lowest byte of c is reserved for the length */
				/* fall through */
			case 8:
				b += ka[1];
				a += ka[0];
				break;
			case 7:
				b += ((int) k[6] << 8);
				/* fall through */
			case 6:
				b += ((int) k[5] << 16);
				/* fall through */
			case 5:
				b += ((int) k[4] << 24);
				/* fall through */
			case 4:
				a += ka[0];
				break;
			case 3:
				a += ((int) k[2] << 8);
				/* fall through */
			case 2:
				a += ((int) k[1] << 16);
				/* fall through */
			case 1:
				a += ((int) k[0] << 24);
				/* case 0: nothing left to add */
		}
#else							/* !WORDS_BIGENDIAN */
		switch (len)
		{
			case 11:
				c += ((int) k[10] << 24);
				/* fall through */
			case 10:
				c += ((int) k[9] << 16);
				/* fall through */
			case 9:
				c += ((int) k[8] << 8);
				/* the lowest byte of c is reserved for the length */
				/* fall through */
			case 8:
				b += ka[1];
				a += ka[0];
				break;
			case 7:
				b += ((int) k[6] << 16);
				/* fall through */
			case 6:
				b += ((int) k[5] << 8);
				/* fall through */
			case 5:
				b += k[4];
				/* fall through */
			case 4:
				a += ka[0];
				break;
			case 3:
				a += ((int) k[2] << 16);
				/* fall through */
			case 2:
				a += ((int) k[1] << 8);
				/* fall through */
			case 1:
				a += k[0];
				/* case 0: nothing left to add */
		}
#endif							/* WORDS_BIGENDIAN */
	}
	else
	{
		/* Code path for non-aligned source data */

		/* handle most of the key */
		while (len >= 12)
		{
#ifdef WORDS_BIGENDIAN
			a += (k[3] + ((int) k[2] << 8) + ((int) k[1] << 16) + ((int) k[0] << 24));
			b += (k[7] + ((int) k[6] << 8) + ((int) k[5] << 16) + ((int) k[4] << 24));
			c += (k[11] + ((int) k[10] << 8) + ((int) k[9] << 16) + ((int) k[8] << 24));
#else							/* !WORDS_BIGENDIAN */
			a += (k[0] + ((int) k[1] << 8) + ((int) k[2] << 16) + ((int) k[3] << 24));
			b += (k[4] + ((int) k[5] << 8) + ((int) k[6] << 16) + ((int) k[7] << 24));
			c += (k[8] + ((int) k[9] << 8) + ((int) k[10] << 16) + ((int) k[11] << 24));
#endif							/* WORDS_BIGENDIAN */
			mix(a, b, c);
			k += 12;
			len -= 12;
		}

		/* handle the last 11 bytes */
#ifdef WORDS_BIGENDIAN
		switch (len)			/* all the case statements fall through */
		{
			case 11:
				c += ((int) k[10] << 8);
			case 10:
				c += ((int) k[9] << 16);
			case 9:
				c += ((int) k[8] << 24);
				/* the lowest byte of c is reserved for the length */
			case 8:
				b += k[7];
			case 7:
				b += ((int) k[6] << 8);
			case 6:
				b += ((int) k[5] << 16);
			case 5:
				b += ((int) k[4] << 24);
			case 4:
				a += k[3];
			case 3:
				a += ((int) k[2] << 8);
			case 2:
				a += ((int) k[1] << 16);
			case 1:
				a += ((int) k[0] << 24);
				/* case 0: nothing left to add */
		}
#else							/* !WORDS_BIGENDIAN */
		switch (len)			/* all the case statements fall through */
		{
			case 11:
				c += ((int) k[10] << 24);
			case 10:
				c += ((int) k[9] << 16);
			case 9:
				c += ((int) k[8] << 8);
				/* the lowest byte of c is reserved for the length */
			case 8:
				b += ((int) k[7] << 24);
			case 7:
				b += ((int) k[6] << 16);
			case 6:
				b += ((int) k[5] << 8);
			case 5:
				b += k[4];
			case 4:
				a += ((int) k[3] << 24);
			case 3:
				a += ((int) k[2] << 16);
			case 2:
				a += ((int) k[1] << 8);
			case 1:
				a += k[0];
				/* case 0: nothing left to add */
		}
#endif							/* WORDS_BIGENDIAN */
	}

	final(a, b, c);

	/* report the result */
	return c;
}
