//	Entry point into the Nachos kernel from user programs.
//	There are two kinds of things that can cause control to
//	transfer back to here from user code:
//
//	syscall -- The user code explicitly requests to call a procedure
//	in the Nachos kernel.  Right now, the only function we support is
//	"Halt".
//
//	exceptions -- The user code does something that the CPU can't handle.
//	For instance, accessing memory that doesn't exist, arithmetic errors,
//	etc.
//
//	Interrupts (which can also cause control to transfer from user
//	code into the Nachos kernel) are handled elsewhere.
//
// For now, this only handles the Halt() system call.
// Everything else core dumps.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"
#include "syscall.h"

//----------------------------------------------------------------------
// ExceptionHandler
// 	Entry point into the Nachos kernel.  Called when a user program
//	is executing, and either does a syscall, or generates an addressing
//	or arithmetic exception.
//
// 	Cac quy dinh su dung thanh ghi:
//
// 	   1. Thanh ghi r2: Chua ma lenh syscall va ket qua thuc hien o syscall se tra nguoc ve
// 	   2. Thanh ghi r4: Tham so thu nhat
//	   3. Thanh ghi r5: Tham so thu hai
//	   4. Thanh ghi r6: Tham so thu ba
//	   5. Thanh ghi r7: Tham so thu tu
//
//
//
// And don't forget to increment the pc before returning. (Or else you'll
// loop making the same system call forever!
//
//	"which" is the kind of exception.  The list of possible exceptions
//	are in machine.h.
//----------------------------------------------------------------------

// Input: Khong gian dia chi User(int) - gioi han cua buffer(int)
// Output: Bo nho dem Buffer(char*)
// Chuc nang: Sao chep vung nho User sang vung nho System
char *User2System(int virtAddr, int limit)
{
	int i; // chi so index
	int oneChar;
	char *kernelBuf = NULL;
	kernelBuf = new char[limit + 1]; // can cho chuoi terminal
	if (kernelBuf == NULL)
		return kernelBuf;

	memset(kernelBuf, 0, limit + 1);

	for (i = 0; i < limit; i++)
	{
		machine->ReadMem(virtAddr + i, 1, &oneChar);
		kernelBuf[i] = (char)oneChar;
		if (oneChar == 0)
			break;
	}
	return kernelBuf;
}

// Input: Khong gian vung nho User(int) - gioi han cua buffer(int) - bo nho dem buffer(char*)
// Output: So byte da sao chep(int)
// Chuc nang: Sao chep vung nho System sang vung nho User
int System2User(int virtAddr, int len, char *buffer)
{
	if (len < 0)
		return -1;
	if (len == 0)
		return len;
	int i = 0;
	int oneChar = 0;
	do
	{
		oneChar = (int)buffer[i];
		machine->WriteMem(virtAddr + i, 1, oneChar);
		i++;
	} while (i < len && oneChar != 0);
	return i;
}

// Di chuyen thanh ghi ve sau 4 byte de nap lenh ke tiep
// Cac buoc:
// B1: Ghi vao PrevPcReg <- gia tri PCReg hien tai
// B2: Ghi vao PCReg hien tai <- gia tri NextPCReg
// B3: Di chuyen thanh ghi ve sau 4 byte (ghi vao NextPCReg <- gia tri NextPCReg + 4)
void Increase_ProgramCounter()
{
	/*
	machine/mipssim.cc cuoi ham "void Machine::OneInstruction(Instruction *instr)"

	 // Advance program counters.
			registers[PrevPCReg] = registers[PCReg];	// for debugging, in case we
													// are jumping into lala-land
			registers[PCReg] = registers[NextPCReg];
			registers[NextPCReg] = pcAfter;
	*/
	int pcAfter = machine->ReadRegister(NextPCReg) + 4;
	machine->WriteRegister(PrevPCReg, machine->ReadRegister(PCReg));
	machine->WriteRegister(PCReg, machine->ReadRegister(NextPCReg));
	machine->WriteRegister(NextPCReg, pcAfter);
}

void Exception_ReadInt()
{

	const int maxlen = 11; // max len(int) = 11 do int: [-2147483648 , 2147483647]
	char num_string[maxlen] = {0};
	long long ret = 0;
	char c = ' ';
	// remove front space
	while (c == ' ')
	{
		gSynchConsole->Read(&c, 1);
	}
	int length = 0;
	// check sign
	bool sign = 0;
	if (c == '-')
	{
		sign = 1;
	}
	else if (c == '+')
	{
		sign = 0;
	}
	else if (c >= '0' && c <= '9')
	{
		num_string[length] = c;
		length++;
	}

	// read full number
	gSynchConsole->Read(&c, 1);
	while (c <= '9' && c >= '0' && length < maxlen)
	{
		num_string[length] = c;
		length++;
		gSynchConsole->Read(&c, 1);
	}

	for (int i = 0; i < length; i++)
	{
		ret = ret * 10 + 1ll * (num_string[i] - '0');
	}
	if (ret > 2147483647)
	{
		ret = 0;
	}
	ret = sign ? (-ret) : ret;

	machine->WriteRegister(2, (int)ret);
}

void Exception_Halt()
{
	DEBUG('a', "\n Shutdown, initiated by user program.");
	printf("\n");
	interrupt->Halt();
}

void Exception_PrintInt()
{
	int n = machine->ReadRegister(4); // đọc số đã được gán sẵn
	const int maxlen = 11;						// giá trị độ dài tối đa kiểu int
	char num_string[maxlen] = {0};
	int tmp[maxlen] = {0}, i = 0, j = 0;
	if (n < 0)
	{												 // xử lý số âm
		n = -n;								 // nếu âm chuyển về dương để xử lý
		num_string[i++] = '-'; // chuỗi số xuất ra thêm dấu âm
	}
	do
	{ // chuyển số sang mảng int để dễ thao tác
		tmp[j++] = n % 10;
		n /= 10;
	} while (n);
	while (j) // chuyển mảng int sang char
		num_string[i++] = '0' + (char)tmp[--j];
	gSynchConsole->Write(num_string, i); // ghi ra màn hình char và độ dài mảng
	machine->WriteRegister(2, 0);
}

void Exception_ReadChar()
{
	char ch = 0;
	gSynchConsole->Read(&ch, 1); // đọc 1 ký tự từ console
	machine->WriteRegister(2, (int)ch);
}

void Exception_PrintChar()
{
	char ch = (char)machine->ReadRegister(4); // lấy ký tự đã được gán sẵn
	gSynchConsole->Write(&ch, 1);							// ghi 1 ký tự ra màn hình console
	machine->WriteRegister(2, 0);
}

void Exception_ReadString()
{
	int addr, maxLength;
	char *buffer;
	addr = machine->ReadRegister(4);
	maxLength = machine->ReadRegister(5);

	gSynchConsole->Read(buffer, maxLength);

	System2User(addr, maxLength, buffer);
}

void Exception_PrintString()
{
	int addr;
	char *buffer;
	addr = machine->ReadRegister(4);

	buffer = User2System(addr, 255);

	gSynchConsole->Write(buffer, strlen(buffer));
	delete[] buffer;
}

void Exception_CreateFile()
{
	int addr;
	char *fileName;

	addr = machine->ReadRegister(4);

	fileName = User2System(addr, 33);

	if (fileName == NULL)
	{
		printf("\nKhong du bo nho");
		DEBUG('a', "\nKhong du bo nho");
		machine->WriteRegister(2, -1); // Nếu lỗi thì ghi vào thanh ghi số 2 giá trị -1
	}

	if (strlen(fileName) == 0)
	{
		printf("\nTen file khong duoc trong");
		DEBUG('a', "\nTen file khong duoc trong");
		machine->WriteRegister(2, -1); // Nếu lỗi thì ghi vào thanh ghi số 2 giá trị -1
	}

	// Create file with size = 0
	// Dùng đối tượng fileSystem của lớp OpenFile để tạo file
	if (!fileSystem->Create(fileName, 0))
	{
		printf("\nXay ra loi khi tao file (%s)", fileName);
		machine->WriteRegister(2, -1);
	}

	machine->WriteRegister(2, 0); // Nếu không có lỗi ghi vào thanh ghi số 2
	delete[] fileName;						// Giải phóng vùng nhớ cho filename
}

void Exception_Open()
{
	int addr;
	int type;
	char *filename;

	// Lấy các tham số từ thanh ghi
	addr = machine->ReadRegister(4);
	type = machine->ReadRegister(5);
	filename = User2System(addr, 255);

	int freeSlot = fileSystem->FindFreeSlot();

	if (freeSlot == -1)
	{
		machine->WriteRegister(2, -1); // Trả lỗi về cho người dùng
		return;
	}

	switch (type)
	{
	case 0:
		fileSystem->openf[freeSlot] = fileSystem->Open(filename, type);
		if (fileSystem->openf[freeSlot] != NULL)
		{
			machine->WriteRegister(2, freeSlot); // trả về vị trí còn trống
		}
		break;
	case 1:
		fileSystem->openf[freeSlot] = fileSystem->Open(filename, type);
		if (fileSystem->openf[freeSlot] != NULL) // Mo file thanh cong
		{
			machine->WriteRegister(2, freeSlot); // trả về vị trí còn trống
		}
		break;
	case 2:
		machine->WriteRegister(2, 0);
		break;
	case 3:
		machine->WriteRegister(2, 1);
		break;
	}

	delete[] filename;
}

void Exception_Close()
{
	int fileId;
	fileId = machine->ReadRegister(4);
	// Chỉ xử lí trong phạm vi bảng mô tả file
	if (fileId >= 0 && fileId <= 10)
	{
		if (fileSystem->openf[fileId])
		{
			machine->WriteRegister(2, 0); // Trả kết quả thành công
			delete[] fileSystem->openf[fileId];
		}
	}
	machine->WriteRegister(2, -1); // Trả lỗi
}

void Exception_Read()
{
	int addr;
	int charCount;
	int id;

	int firstPositionInFile;
	int lastPositionInFile;
	char *buffer;
	char *temp;

	// Lấy giá trị tham số từ thanh ghi
	addr = machine->ReadRegister(4);
	charCount = machine->ReadRegister(5);
	id = machine->ReadRegister(6);

	// Nếu nằm ngoài bảng mô tả thì trả lỗi
	if (id < 0 || id > 14)
	{
		printf("\nFile ID nam ngoai bang mo ta");
		machine->WriteRegister(2, -1);
		return;
	}

	// Kiểm tra file tồn tại
	if (fileSystem->openf[id] == NULL)
	{
		printf("\nFile khong ton tai.");
		machine->WriteRegister(2, -1);
		return;
	}

	if (fileSystem->openf[id]->type == 3)
	{
		printf("\nKhong the read file stdout.");
		machine->WriteRegister(2, -1);
		return;
	}

	firstPositionInFile = fileSystem->openf[id]->GetCurrentPos();
	temp = User2System(addr, charCount);

	// file stdin
	if (fileSystem->openf[id]->type == 2)
	{
		// Số byte thực sự đọc được
		int size = gSynchConsole->Read(temp, charCount);
		System2User(addr, size, temp);
		machine->WriteRegister(2, size);
		delete temp;
		return;
	}

	if ((fileSystem->openf[id]->Read(temp, charCount)) > 0)
	{
		lastPositionInFile = fileSystem->openf[id]->GetCurrentPos();
		// Số byte thực sự = lastPositionInFile - firstPositionInFile
		System2User(addr, lastPositionInFile - firstPositionInFile, temp);
		machine->WriteRegister(2, lastPositionInFile - firstPositionInFile);
	}
	else
	{
		// Đến cuối file
		machine->WriteRegister(2, -2);
	}
	delete temp;
}

void Exception_Write()
{
	int virtAddr = machine->ReadRegister(4);	// Lay dia chi cua tham so buffer tu thanh ghi so 4
	int charcount = machine->ReadRegister(5); // Lay charcount tu thanh ghi so 5
	int id = machine->ReadRegister(6);				// Lay id cua file tu thanh ghi so 6
	int OldPos;
	int NewPos;
	char *temp;

	if (id < 0 || id > 14)
	{
		printf("\nKhong the write vi id nam ngoai bang mo ta file.");
		machine->WriteRegister(2, -1);
	}

	if (fileSystem->openf[id] == NULL)
	{
		printf("\nKhong the write vi file nay khong ton tai.");
		machine->WriteRegister(2, -1);
	}

	// file only read hoac file stdin thi tra ve -1
	if (fileSystem->openf[id]->type == 1 || fileSystem->openf[id]->type == 2)
	{
		printf("\nKhong the write file stdin hoac file only read.");
		machine->WriteRegister(2, -1);
	}
	OldPos = fileSystem->openf[id]->GetCurrentPos();
	temp = User2System(virtAddr, charcount);

	// file read & write thì trả về số byte thực sự
	if (fileSystem->openf[id]->type == 0)
	{
		if ((fileSystem->openf[id]->Write(temp, charcount)) > 0)
		{
			// Số byte thực sự = NewPos - OldPos
			NewPos = fileSystem->openf[id]->GetCurrentPos();
			machine->WriteRegister(2, NewPos - OldPos);
			delete temp;
		}
	}

	// Trường hợp còn lại là file stdout
	if (fileSystem->openf[id]->type == 3)
	{
		int i = 0;
		while (temp[i] != 0 && temp[i] != '\n')
		{
			gSynchConsole->Write(temp + i, 1);
			i++;
		}
		temp[i] = '\n';
		gSynchConsole->Write(temp + i, 1);
		machine->WriteRegister(2, i - 1); // trả về số byte thực sự
		delete temp;
	}
}

void Exception_Exec()
{
	int virtAddr;
	virtAddr = machine->ReadRegister(4);
	char *name;
	name = User2System(virtAddr, 33);

	if (name == NULL)
	{
		printf("\n Khong du vung nho trong he thong");
		machine->WriteRegister(2, -1);
		return;
	}

	OpenFile *oFile = fileSystem->Open(name);

	if (oFile == NULL)
	{
		printf("\nKhong the mo file");
		machine->WriteRegister(2, -1);
		return;
	}

	delete oFile;

	int id = pTab->ExecUpdate(name);
	machine->WriteRegister(2, id);

	delete[] name;
}

void Exception_Join()
{
	int id = machine->ReadRegister(4);

	int res = pTab->JoinUpdate(id);

	machine->WriteRegister(2, res);
}

void Exception_Exit()
{
	int exitstatus = machine->ReadRegister(4);
	machine->WriteRegister(2, pTab->ExitUpdate(exitstatus));
}

void Exception_CreateSemaphore()
{
	int addr;
	int semval;

	// Lấy các tham số từ thanh ghi
	addr = machine->ReadRegister(4);
	semval = machine->ReadRegister(5);

	char *name = User2System(addr, 255);
	if (name == NULL)
	{
		DEBUG('a', "\nKhong du bo nho");
		printf("\nKhong du bo nho");
		machine->WriteRegister(2, -1);
		delete[] name;
		return;
	}

	if (semTab->Create(name, semval) == -1)
	{
		DEBUG('a', "\nCo loi khi khoi tao semaphore");
		printf("\nCo loi khi khoi tao semaphore");
		machine->WriteRegister(2, -1);
		delete[] name;
		return;
	}

	delete[] name;
	machine->WriteRegister(2, 0);
}

void Exception_Wait()
{
	int addr;

	addr = machine->ReadRegister(4);

	char *name = User2System(addr, 255);
	if (name == NULL)
	{
		DEBUG('a', "\nKhong du bo nho");
		printf("\nKhong du bo nho");
		machine->WriteRegister(2, -1);
		delete[] name;
		return;
	}

	int result = semTab->Wait(name);

	if (result == -1)
	{
		DEBUG('a', "\n Khong ton tai ten semaphore nay!");
		printf("\n Khong ton tai ten semaphore nay!");
	}

	delete[] name;
	machine->WriteRegister(2, result);
}

void Exception_Signal()
{
	int addr;
	char *name;

	addr = machine->ReadRegister(4);
	name = User2System(addr, 255);
	if (name == NULL)
	{
		DEBUG('a', "\nKhong du bo nho");
		printf("\nKhong du bo nho");
		machine->WriteRegister(2, -1);
		delete[] name;
		return;
	}

	int result = semTab->Signal(name);

	if (result == -1)
	{
		DEBUG('a', "\n Khong ton tai ten semaphore nay!");
		printf("\n Khong ton tai ten semaphore nay!");
	}

	delete[] name;
	machine->WriteRegister(2, result);
}

// Ham xu ly ngoai le runtime Exception va system call
void ExceptionHandler(ExceptionType which)
{
	int type = machine->ReadRegister(2);
	switch (which)
	{
	case NoException:
		return;

	case PageFaultException:
		DEBUG('a', "\nKhong tim thay dinh vi du lieu trong RAM. ");
		printf("\n\nKhong tim thay dinh vi du lieu trong RAM. ");
		interrupt->Halt();
		break;
	case ReadOnlyException:
		DEBUG('a', "\nTrang dang co gang ghi vao duoc danh dau la 'chi doc'.");
		printf("\n\nTrang dang co gang ghi vao duoc danh dau la 'chi doc'.");
		interrupt->Halt();
		break;
	case BusErrorException:
		DEBUG('a', "\nChuong trinh dang co truy cap bo nho khong the xac dinh dia chi vat ly.");
		printf("\n\nChuong trinh dang co truy cap bo nho khong the xac dinh dia chi vat ly.");
		interrupt->Halt();
		break;
	case AddressErrorException:
		DEBUG('a', "\nChuong trinh dang tham chieu den mot khong gian dia chi bat hop phap.");
		printf("\n\nChuong trinh dang tham chieu den mot khong gian dia chi bat hop phap.");
		interrupt->Halt();
		break;
	case OverflowException:
		DEBUG('a', "\nLoi tran so do phep tinh cong hoac tru.");
		printf("\n\nLoi tran so do phep tinh cong hoac tru.");
		interrupt->Halt();
		break;
	case IllegalInstrException:
		DEBUG('a', "\nLenh dang co gang thuc thi khong duoc ho tro.");
		printf("\n\nLenh dang co gang thuc thi khong duoc ho tro.");
		interrupt->Halt();
		break;
	case NumExceptionTypes:
		DEBUG('a', "\nChuong trinh gap loi ngoai le so.");
		printf("\n\nChuong trinh gap loi ngoai le so.");
		interrupt->Halt();
		break;

	case SyscallException:
		switch (type)
		{
		case SC_Sum:
		{
			// Code mẫu
			int a = machine->ReadRegister(4);
			int b = machine->ReadRegister(5);

			machine->WriteRegister(2, a + b);
			Increase_ProgramCounter();
			break;
		}

		case SC_Halt:
			DEBUG('a', "\nShutdown, initiated by user program. ");
			printf("\nShutdown, initiated by user program. ");
			interrupt->Halt();
			break;

		case SC_ReadInt:
			Exception_ReadInt();
			Increase_ProgramCounter();
			break;

		case SC_PrintInt:
			Exception_PrintInt();
			Increase_ProgramCounter();
			break;

		case SC_ReadChar:
			Exception_ReadChar();
			Increase_ProgramCounter();
			break;

		case SC_PrintChar:
			Exception_PrintChar();
			Increase_ProgramCounter();
			break;

		case SC_ReadString:
			Exception_ReadString();
			Increase_ProgramCounter();
			break;

		case SC_PrintString:
			Exception_PrintString();
			Increase_ProgramCounter();
			break;

		case SC_CreateFile:
			Exception_CreateFile();
			Increase_ProgramCounter();
			break;

		case SC_Open:
			Exception_Open();
			Increase_ProgramCounter();
			break;

		case SC_Close:
			Exception_Close();
			Increase_ProgramCounter();
			break;

		case SC_Read:
			Exception_Read();
			Increase_ProgramCounter();
			break;

		case SC_Write:
			Exception_Write();
			Increase_ProgramCounter();
			break;

		case SC_Exec:
			Exception_Exec();
			Increase_ProgramCounter();
			break;
		case SC_Join:
			Exception_Join();
			Increase_ProgramCounter();
			break;

		case SC_Exit:
			Exception_Exit();
			Increase_ProgramCounter();
			break;

		case SC_CreateSemaphore:
			Exception_CreateSemaphore();
			Increase_ProgramCounter();
			break;

		case SC_Wait:
			Exception_Wait();
			Increase_ProgramCounter();
			break;

		case SC_Signal:
			Exception_Signal();
			Increase_ProgramCounter();
			break;

		default:
			break;
		}
	}
}