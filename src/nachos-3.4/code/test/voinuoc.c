#include "syscall.h"

int main()
{
	int fileOut; // id File output.txt

	int flag;
	int V;
	int len;
	char c[1];
	int v1, v2; // số lít cần rót tương ứng vòi 1 và vòi 2

	fileOut = Open("./test/output.txt", 0);
	if (fileOut == -1)
	{
		PrintString("\nCan't open file output.txt!");
		Exit(-1);
	}

	do
	{
		flag = Read(c, 1, fileOut);
		if (flag == -1)
		{
			PrintString("\nRead error in file sinhvien.txt!");
			Close(fileOut);
			Exit(-1);
		}
	} while (flag != -2);

	v1 = 0;
	v2 = 0;
	while (1)
	{
		Wait("sinhvien");
		V = 0;
		c[0] = '0';
		do
		{
			V = V * 10 + c[0] - '0';
			flag = Read(c, 1, fileOut);
			if (flag == -1)
			{
				PrintString("\nRead error in file output.txt!");
				Close(fileOut);
				Exit(-1);
			}
			if (flag == -2 || flag == 0)
				break;
		} while (c[0] >= '0' && c[0] <= '9');

		if (V == 0)
			break;

		if (v1 == 0)
		{
			flag = Write("1 ", 2, fileOut);
			if (flag == -1)
			{
				PrintString("\nWrite error in file output.txt!");
				Close(fileOut);
				Exit(-1);
			}

			if (V > v2)
			{
				v1 = V - v2;
				v2 = 0;
			}
			else
			{
				v2 = v2 - V;
			}
		}
		else
		{
			if (Write("2 ", 2, fileOut) == -1)
			{
				PrintString("\nWrite error in file output.txt!");
				Close(fileOut);
				Exit(-1);
			}

			if (V > v1)
			{
				v2 = V - v1;
				v1 = 0;
			}
			else
			{
				v1 = v1 - V;
			}
		}
		Signal("voinuoc");
	}

	c[0] = '\n';
	if (Write(c, 1, fileOut) == -1)
	{
		PrintString("\nWrite error in file output.txt!");
		Close(fileOut);
		Exit(-1);
	}

	Signal("main");

	Close(fileOut);
	Exit(0);
}
