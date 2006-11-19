/*
 * Random number generation.
 * The algorithms and magic numbers, but not the code, are copied from
 * xscreensaver.
 */

#include <time.h>

#define VectorSize 55
static unsigned int a[VectorSize] =
{
 035340171546, 010401501101, 022364657325, 024130436022, 002167303062, /*  5 */
 037570375137, 037210607110, 016272055420, 023011770546, 017143426366, /* 10 */
 014753657433, 021657231332, 023553406142, 004236526362, 010365611275, /* 14 */
 007117336710, 011051276551, 002362132524, 001011540233, 012162531646, /* 20 */
 007056762337, 006631245521, 014164542224, 032633236305, 023342700176, /* 25 */
 002433062234, 015257225043, 026762051606, 000742573230, 005366042132, /* 30 */
 012126416411, 000520471171, 000725646277, 020116577576, 025765742604, /* 35 */
 007633473735, 015674255275, 017555634041, 006503154145, 021576344247, /* 40 */
 014577627653, 002707523333, 034146376720, 030060227734, 013765414060, /* 45 */
 036072251540, 007255221037, 024364674123, 006200353166, 010126373326, /* 50 */
 015664104320, 016401041535, 016215305520, 033115351014, 017411670323  /* 55 */
};

static int i1, i2;

static unsigned int ya_random(void)
{
	int ret = a[i1] + a[i2];
	a[i1] = ret;
	if (++i1 >= VectorSize) i1 = 0;
	if (++i2 >= VectorSize) i2 = 0;
	return ret;
}

static void ya_rand_init_with_seed(unsigned int seed)
{
	int i;
	a[0] += seed;
	for (i = 1; i < VectorSize; i++)
	{
		seed = 1001*a[i-1] + 999*seed;
		a[i] += seed;
	}
	i1 = a[0] % VectorSize;
	i2 = (i1 + 20) % VectorSize;
}

static void rand_init(void)
{
	ya_rand_init_with_seed((unsigned int)time(NULL));
}

static int rand_inited = 0;

static void check_init(void)
{
	if (!rand_inited)
	{
		rand_init();
		rand_inited = 1;
	}
}

int rnd(int n)
{
	check_init();
	return (int)(ya_random()>>1) % n;
}

double frnd(void)
{
	check_init();
	return (ya_random() & 0xffffff) / 16777215.0f;
}
