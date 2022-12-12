#include "syscall.h"

int main()
{
	int fileIn;				// id File input.txt
	int fileSinhVien; // id File trung gian

	SpaceId sinhvien_proc, voinuoc_proc; // space id của chương trình sinhvien va voinuoc

	int N;
	int flag;
	char c[1];

	if (CreateSemaphore("main", 1) == -1)
	{
		PrintString("\nCreate semaphore failed!");
		Exit(-1);
	}

	if (CreateSemaphore("sinhvien", 0) == -1)
	{
		PrintString("\nCreate semaphore failed!");
		Exit(-1);
	}

	if (CreateSemaphore("voinuoc", 1) == -1)
	{
		PrintString("\nCreate semaphore failed!");
		Exit(-1);
	}

	flag = CreateFile("./test/output.txt");
	if (flag == -1)
	{
		PrintString("\nCan't create file output.txt!");
		Exit(-1);
	}

	fileIn = Open("./test/input.txt", 1);
	if (fileIn == -1)
	{
		PrintString("\nCan't open file input.txt!");
		Exit(-1);
	}

	N = 0;
	c[0] = '0';

	do
	{
		N = N * 10 + c[0] - '0';
		if (Read(c, 1, fileIn) == -1)
		{
			PrintString("\nRead error in file input.txt!");
			Close(fileIn);
			Exit(-1);
		}
	} while (c[0] >= '0' && c[0] <= '9');

	while (N--)
	{
		flag = CreateFile("./test/sinhvien.txt");
		if (flag == -1)
		{
			PrintString("\nCan't create file sinhvien.txt!");
			Close(fileIn);
			Exit(-1);
		}

		fileSinhVien = Open("./test/sinhvien.txt", 0);
		if (fileSinhVien == -1)
		{
			PrintString("\nmain.c");
			PrintString("\nCan't open file sinhvien.txt!");
			Close(fileIn);
			Exit(-1);
		}

		Wait("main");
		while (1)
		{
			flag = Read(c, 1, fileIn);
			if (flag == 0 || flag == -2)
				break;
			if (flag == -1)
			{
				PrintString("\nRead error in file input.txt!");
				Close(fileIn);
				Close(fileSinhVien);
				Exit(-1);
			}
			if (c[0] == '\n')
				break;

			if (Write(c, 1, fileSinhVien) == -1)
			{
				PrintString("\nWrite error in file sinhvien.txt!");
				Close(fileIn);
				Close(fileSinhVien);
				Exit(-1);
			}
		}
		Close(fileSinhVien);

		sinhvien_proc = Exec("./test/sinhvien");
		if (sinhvien_proc == -1)
		{
			PrintString("\nCan't execute process sinhvien\n");
			Close(fileIn);
			Exit(1);
		}

		voinuoc_proc = Exec("./test/voinuoc");
		if (voinuoc_proc == -1)
		{
			PrintString("\nCan't execute process voinuoc\n");
			Close(fileIn);
			Exit(1);
		}

		if (Join(sinhvien_proc) == -1)
		{
			PrintString("\nCan't join process sinhvien!");
			Close(fileIn);
			Exit(-1);
		}

		if (Join(voinuoc_proc) == -1)
		{
			PrintString("\nCan't join process voinuoc!");
			Close(fileIn);
			Exit(-1);
		}
	}

	Close(fileIn);
	Exit(0);
}
