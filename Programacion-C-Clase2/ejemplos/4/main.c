#include <stdlib.h>

int main()
{
	int i;
	int *v;
	v = (int *)malloc(10 * sizeof(int));
	for (i = 0; i < 20; i++)
		v[i] = 10;

	// free(v);
}
