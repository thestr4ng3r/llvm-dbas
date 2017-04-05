
#include <stdio.h>

int main()
{
	int a=42;

	if(a < 30)
	{
		printf("1");
	}
	else if(a == 40)
	{
		printf("2");
	}
	else
	{
		a++;
		printf("3");
	}

	return 0;
}