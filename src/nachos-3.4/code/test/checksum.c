#include "syscall.h"

int main()
{

	int a, b, sum;
	a = 5;
	b = 4;
	sum = Sum(a, b);
	PrintString("Sum: ");
	PrintInt(sum);
	PrintString("\n");
	return 0;
}
