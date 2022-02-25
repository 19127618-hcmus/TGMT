#include "lib.h"

bool compare(const char* a, string b)
{
	int la = strlen(a), lb = b.length();
	//cout << la << " - " << lb;
	if (la != lb) return 0;
	else
	{
		for (int i = 0; i < la; i++)
		{
			if (a[i] != b[i]) return 0;
		}
	}

	return 1;
}

float char2float(char* argv[], int n)
{
	float temp = strtod(argv[n], NULL);
	return temp;
}