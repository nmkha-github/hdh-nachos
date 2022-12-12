/*
	Chuong trinh sinhvien cua bai toan 2 voi nuoc
	Ghi dung tich cac binh nuoc vao file output.txt
*/

#include "syscall.h"

int main()
{
	int fileOut;			// id File output.txt
	int fileSinhVien; // id file trung gian

	int flag;
	int stop;
	char V[10]; // Dung tich binh nuoc (chuoi)
	int len;		// Do dai cua chuoi V
	char c[1];

	fileSinhVien = Open("./test/sinhvien.txt", 1);
	if (fileSinhVien == -1)
	{
		PrintString("\nsinhvien.c");
		PrintString("\nCan't open file sinhvien.txt!");
		Exit(-1);
	}

	fileOut = Open("./test/output.txt", 0);
	if (fileOut == -1)
	{
		PrintString("\nCan't open file output.txt!");
		Close(fileSinhVien);
		Exit(-1);
	}

	// Seek: Di chuyen toi cuoi file output
	do
	{
		flag = Read(c, 1, fileOut);
		if (flag == -1)
		{
			PrintString("\nRead error in file sinhvien.txt!");
			Close(fileSinhVien);
			Close(fileOut);
			Exit(-1);
		}
	} while (flag != -2);

	stop = 0;
	do
	{
		len = 0;
		while (1)
		{
			flag = Read(c, 1, fileSinhVien);
			if (flag == -1)
			{
				PrintString("\nRead error in file sinhvien.txt!");
				Close(fileSinhVien);
				Close(fileOut);
				Exit(-1);
			}
			if (flag == -2 || c[0] == ' ' || c[0] == '\n')
				break;

			V[len] = c[0];
			len++;
		}
		V[len] = ' ';
		len++;

		if (flag == -2)
			stop = 1;

		Wait("voinuoc");
		do
		{
			flag = Read(c, 1, fileOut);
			if (flag == -1)
			{
				PrintString("\nRead error in file sinhvien.txt!");
				Close(fileSinhVien);
				Close(fileOut);
				Exit(-1);
			}
		} while (flag != -2);

		if (Write(V, len, fileOut) == -1)
		{
			PrintString("\nWrite error in file sinhvien.txt!");
			Close(fileSinhVien);
			Close(fileOut);
			Exit(-1);
		}
		Signal("sinhvien");
	} while (!stop);

	Signal("sinhvien"); // for final loop of voinuoc

	Close(fileSinhVien);
	Close(fileOut);
	Exit(0);
}
