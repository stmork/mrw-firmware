#include <stdio.h>
#include <stdlib.h>

#define MAX 32

static unsigned char nom[MAX];
static unsigned char denom[MAX];

static void dump(unsigned char * array, char * name)
{
	int i;

	printf ("static const uint8_t %s[%d] PROGMEM =\n{\n\t", name, MAX);
	for (i = 0; i < MAX; i++)
	{
		printf("0x%02x", array[i]);
		if (i < (MAX - 1))
		{
			putchar(',');
		}
		printf((i & 15) != 15 ? " " : "\n\t");
	}
	printf("};\n");
}

int main(int argc, char * argv[])
{
	int i, k;

	denom[0] = MAX;
	for (i = 1; i < MAX; i++)
	{
		unsigned short d = MAX;
		unsigned short n = i;

		for (k = 0; (k < 8) && ((n & 1) == 0); k++)
		{
			n >>= 1;
			d >>= 1;
		}
		nom[i]   = n;
		denom[i] = d;
	}
	dump(nom,   "table_nom");
	dump(denom, "table_denom");
	return EXIT_SUCCESS;
}
