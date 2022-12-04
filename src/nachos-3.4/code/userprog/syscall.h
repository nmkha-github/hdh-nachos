/* syscalls.h 
 * 	Nachos system call interface.  These are Nachos kernel operations
 * 	that can be invoked from user programs, by trapping to the kernel
 *	via the "syscall" instruction.
 *
 *	This file is included by user programs and by the Nachos kernel. 
 *
 * Copyright (c) 1992-1993 The Regents of the University of California.
 * All rights reserved.  See copyright.h for copyright notice and limitation 
 * of liability and disclaimer of warranty provisions.
 */

#ifndef SYSCALLS_H
#define SYSCALLS_H

#include "copyright.h"

#define SC_Halt			0

#define SC_Exit			1
#define SC_Exec			2
#define SC_Join			3
//Syscall file
#define SC_CreateFile		4
#define SC_Open			5
#define SC_Read			6
#define SC_Write		7
#define SC_Close		8
//Syscall cho multithreading
#define SC_Fork			9
#define SC_Yield		10
//Syscall project 2
#define SC_ReadInt		11
#define SC_PrintInt		12
#define SC_ReadChar		13
#define SC_PrintChar		14
#define SC_ReadString		15
#define SC_PrintString		16
#define SC_Seek			17
#define SC_Sum			30

// Syscall Semaphore
#define SC_CreateSemaphore	18
#define SC_Wait			19
#define SC_Signal		20
#ifndef IN_ASM


/* The system call interface.  These are the operations the Nachos
 * kernel needs to support, to be able to run user programs.
 *
 * Each of these is invoked by a user program by simply calling the 
 * procedure; an assembly language stub stuffs the system call code
 * into a register, and traps to the kernel.  The kernel procedures
 * are then invoked in the Nachos kernel, after appropriate error checking, 
 * from the system call entry point in exception.cc.
 */
 
int Sum(int a, int b);

// Address space control operations: Exit, Exec, and Join
void Exit(int status);	

// A unique identifier for an executing user program (address space)
typedef int SpaceId;	

SpaceId Exec(char *name);

int Join(SpaceId id); 	

/* Fork a thread to run a procedure ("func") in the *same* address space 
 * as the current thread.
 */
void Fork(void(*func)());
/* Yield the CPU to another runnable thread, whether in this address space 
 * or not. 
 */
void Yield();

void Halt();

int ReadInt();

void PrintInt(int number);

char ReadChar();

void PrintChar(char character);

void ReadString(char buffer[], int length);

void PrintString(char buffer[]);

// Kiểu dữ liệu đặc biệt cho file
typedef int OpenFileId;

#define ConsoleInput	0  
#define ConsoleOutput	1  

int CreateFile(char *name);

OpenFileId Open(char *name, int type);

/* Close the file, we're done reading and writing to it. */
void Close(OpenFileId id);

/* Read "size" bytes from the open file into "buffer".  
 * Return the number of bytes actually read -- if the open file isn't
 * long enough, or if it is an I/O device, and there aren't enough 
 * characters to read, return whatever is available (for I/O devices, 
 * you should always wait until you can return at least one character).
 */
int Read(char *buffer, int charcount, OpenFileId id);

// Write "size" bytes from "buffer" to the open file.
int Write(char *buffer, int charcount, OpenFileId id);

int Seek(int pos, OpenFileId id);


// semaphore
int CreateSemaphore(char* name, int semval);

int Wait(char* name);

int Signal(char* name);


#endif /* IN_ASM */

#endif /* SYSCALL_H */
