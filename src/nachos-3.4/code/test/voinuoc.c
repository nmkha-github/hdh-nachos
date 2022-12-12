/*
	Chuong trinh voinuoc cua bai toan 2 voi nuoc
*/

#include "syscall.h"

int main() {
	// ------------------------ Khai bao cac bien dung trong chuong trinh ---------------------------------------
	int fileOut;							// File output.txt

	int flag;								// Ghi nhan ket qua cac ham thanh cong hay that bai
	int V;									// Dung tich binh nuoc
	int len;								// Do dai cua chuoi V
	char c[1]; 								// Dung de doc ghi file
	int v1, v2;								// So lit can rot o voi 1 va voi 2 tai thoi diem dang xet


	// ------------------------ Khoi tao file, neu khong thanh cong thi dung chuong trinh ------------------------
	fileOut = Open("./test/output.txt", 0);
	if (fileOut == -1) {
		PrintString("\nCan't open file output.txt!");
		Exit(-1);
	}

	// Seek: Di chuyen toi cuoi file output
	do {
		flag = Read(c, 1, fileOut);
		if (flag == -1) {
			PrintString("\nRead error in file sinhvien.txt!");
			Close(fileOut);
			Exit(-1);
		}
	} while (flag !=-2);


	// ------------------------ Phan than chuong trinh: lay dung tich tung binh nuoc, ghi STT cua voi nuoc -------
	v1 = 0;
	v2 = 0;
	while (1) {
		Wait("sinhvien");
		// ====== Critical Section START ======
		// Doc dung tich binh nuoc tu file output.txt (do CT sinhvien ghi vao)
		V = 0;
		c[0] = '0';
		do {
			V = V * 10 + c[0] - '0';
			flag = Read(c, 1, fileOut);
			if (flag == -1) {
				PrintString("\nRead error in file output.txt!");
				Close(fileOut);
				Exit(-1);
			}
			if (flag == -2 || flag == 0) break;
		} while(c[0]>='0' && c[0]<='9');

		// Neu sinhvien khong ghi them vao file output nua (ket thuc testcase), thi dung vong lap

		if (V == 0) break;

		// Ghi ra file output.txt ket qua
		// Neu v1 dang ranh, thi tat nhien rot bang voi 1; Tinh toan v1, v2 sau khi rot xong
		if (v1 == 0) {
			flag = Write("1 ", 2, fileOut);
			if (flag == -1) {
				PrintString("\nWrite error in file output.txt!");
				Close(fileOut);
				Exit(-1);
			}

			if (V > v2) {
				v1 = V - v2;
				v2 = 0;
			}
			else {
				v2 = v2 - V;
			}
		}
		// Nguoc lai rot bang voi 2
		else {
			flag = Write("2 ", 2, fileOut);
			if (flag == -1) {
				PrintString("\nWrite error in file output.txt!");
				Close(fileOut);
				Exit(-1);
			}

			if (V > v1) {
				v2 = V - v1;
				v1 = 0;
			}
			else {
				v1 = v1 - V;
			}
		}
		// ====== Critical Section END ========
		Signal("voinuoc");
	}
	// Xuong dong sau khi xong testcase

	c[0]='\n';
	flag = Write(c, 1, fileOut);
	if (flag == -1) {
		PrintString("\nWrite error in file output.txt!");
		Close(fileOut);
		Exit(-1);
	}

	// Ra hieu cho chuong trinh main tiep tuc testcase ke tiep
	Signal("main");

	Close(fileOut);
	Exit(0);	
}
