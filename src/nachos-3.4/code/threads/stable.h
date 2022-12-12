#ifndef STABLE_H
#define STABLE_H
#include "synch.h"
#include "bitmap.h"
#define MAX_SEMAPHORE 10

class Sem
{
private:
	char name[50];
	Semaphore *sem;

public:
	Sem(char *na, int i)
	{
		strcpy(this->name, na);
		sem = new Semaphore(this->name, i);
	}

	~Sem()
	{
		if (sem != NULL)
		{
			delete sem;
		}
	}

	void wait()
	{
		sem->P();
	}

	void signal()
	{
		sem->V();
	}

	char *GetName()
	{
		return this->name;
	}
};

class STable
{
private:
	BitMap *bm;									// quản lý slot trống
	Sem *semTab[MAX_SEMAPHORE]; // quản lý tối đa 10 đối tượng Sem
public:
	STable();

	~STable();
	int Create(char *name, int init);

	int Wait(char *name);

	int Signal(char *name);

	int FindFreeSlot();
};
#endif
