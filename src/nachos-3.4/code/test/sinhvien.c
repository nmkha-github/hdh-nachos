/*
	Chuong trinh sinhvien cua bai toan 2 voi nuoc
	Ghi dung tich cac binh nuoc vao file output.txt
*/

#include "syscall.h"

int main()
{
	// ------------------------ Khai bao cac bien dung trong chuong trinh ---------------------------------------
	int fileOut;			// File output.txt
	int fileSinhVien; // File tam, truyen tham so tu main cho sinhvien

	int flag;		// Ghi nhan ket qua cac ham thanh cong hay that bai
	int stop;		// Flag dung chuong trinh khi doc den cuoi file
	char V[10]; // Dung tich binh nuoc (chuoi)
	int len;		// Do dai cua chuoi V
	char c[1];	// Dung de doc ghi file

	// ------------------------ Khoi tao file, neu khong thanh cong thi dung chuong trinh ------------------------
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

	// ------------------------ Phan than chuong trinh: lay dung tich tung binh nuoc, dat vao voi nuoc -----------
	stop = 0;
	do
	{
		// Doc dung tich tu file trung gian (het file thi dung)
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
		len++; // Chen them dau cach de phan cach cac so

		if (flag == -2)
			stop = 1;

		// Ghi dung tich vao cuoi file output.txt
		Wait("voinuoc");
		// ====== Critical Section START ======
		// Seek: di chuyen toi cuoi file output.txt
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

		// Ghi dung tich vao file
		flag = Write(V, len, fileOut);
		if (flag == -1)
		{
			PrintString("\nWrite error in file sinhvien.txt!");
			Close(fileSinhVien);
			Close(fileOut);
			Exit(-1);
		}
		// ====== Critical Section END ========
		Signal("sinhvien");
	} while (!stop);

	Signal("sinhvien"); // for final loop of voinuoc

	Close(fileSinhVien);
	Close(fileOut);
	Exit(0);
}
