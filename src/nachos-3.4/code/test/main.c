/*
	Chuong trinh chinh cua bai toan 2 voi nuoc
	Doc file input, exec cac chuong trinh sinhvien va voinuoc, cho hai chuong trinh nay ket thuc
*/

#include "syscall.h"

int main() {
	// ------------------------ Khai bao cac bien dung trong chuong trinh ---------------------------------------
	int fileIn;								// File input.txt
	int fileSinhVien; 						// File tam, truyen tham so tu main cho sinhvien

	SpaceId sinhvien_proc, voinuoc_proc;	// space id cua hai chuong trinh sinhvien va voinuoc

	int N;									// So testcase - thoi diem can xet (doc tu input)
	int flag;								// Ghi nhan ket qua cac ham thanh cong hay that bai
	char c[1]; 								// Dung de doc ghi file


	// ------------------------ Khoi tao semaphore, neu khong thanh cong thi dung chuong trinh -------------------
	flag = CreateSemaphore("main", 1);
	if (flag == -1) {
		PrintString("\nCreate semaphore failed!");
		Exit(-1);
	}

	flag = CreateSemaphore("sinhvien", 0);
	if (flag == -1) {
		PrintString("\nCreate semaphore failed!");
		Exit(-1);
	}

	flag = CreateSemaphore("voinuoc", 1);
	if (flag == -1) {
		PrintString("\nCreate semaphore failed!");
		Exit(-1);
	}


	// ------------------------ Khoi tao file, neu khong thanh cong thi dung chuong trinh ------------------------
	flag = CreateFile("./test/output.txt");
	if (flag == -1) {
		PrintString("\nCan't create file output.txt!");
		Exit(-1);
	}

	fileIn = Open("./test/input.txt", 1);
	if (fileIn == -1) {
		PrintString("\nCan't open file input.txt!");
		Exit(-1);
	}


	// ------------------------ Doc tu file so luong thoi diem N -------------------------------------------------
	N = 0;
	c[0] = '0';

	do {
		N = N * 10 + c[0] - '0';
		if (Read(c, 1, fileIn) == -1) {
			PrintString("\nRead error in file input.txt!");
			Close(fileIn);
			Exit(-1);
		}
	} while(c[0]>='0' && c[0]<='9');


	// ------------------------ Phan than chuong trinh: lay du lieu tung thoi diem, theo doi cac Chuong trinh ----
	while (N--) {
		// Buoc 0: Tao file trung gian truyen du lieu giua main va sinhvien
		flag = CreateFile("./test/sinhvien.txt");
		if (flag == -1) {
			PrintString("\nCan't create file sinhvien.txt!");
			Close(fileIn);
			Exit(-1);
		}

		fileSinhVien = Open("./test/sinhvien.txt", 0);
		if (fileSinhVien == -1) {
			PrintString("\nCan't open file sinhvien.txt!");
			Close(fileIn);
			Exit(-1);
		}

		// Buoc 1: Doc mot dong du lieu tu file input (tuong ung voi mot testcase), ghi ra file trung gian sinhvien.txt
		Wait("main");
        while (1) {
			// DOC
			flag = Read(c, 1, fileIn);
            if (flag == 0 || flag == -2) break;
			if (flag == -1) {
				PrintString("\nRead error in file input.txt!");
				Close(fileIn);
				Close(fileSinhVien);
				Exit(-1);
			}
            if (c[0] == '\n') break;

			// GHI
            flag = Write(c, 1, fileSinhVien);
			if (flag == -1) {
				PrintString("\nWrite error in file sinhvien.txt!");
				Close(fileIn);
				Close(fileSinhVien);
				Exit(-1);
			}
        }
		Close(fileSinhVien);
		
		// Buoc 2: Thuc thi va join cac chuong trinh
		sinhvien_proc = Exec("./test/sinhvien");
		if (sinhvien_proc == -1) {
		    PrintString("\nCan't execute process sinhvien\n");
			Close(fileIn);
		    Exit(1);
		}

		voinuoc_proc = Exec("./test/voinuoc");
		if (voinuoc_proc == -1) {
		    PrintString("\nCan't execute process voinuoc\n");
			Close(fileIn);
		    Exit(1);
		}

		flag = Join(sinhvien_proc);
		if (flag == -1) {
			PrintString("\nCan't join process sinhvien!");
			Close(fileIn);
			Exit(-1);
		}

		flag = Join(voinuoc_proc);
		if (flag == -1) {
			PrintString("\nCan't join process voinuoc!");
			Close(fileIn);
			Exit(-1);
		}
	}

	Close(fileIn);
	Exit(0);
}
