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

double char2double(char* argv[], int n)
{
	double temp = strtod(argv[n], NULL);
	return temp;
}
