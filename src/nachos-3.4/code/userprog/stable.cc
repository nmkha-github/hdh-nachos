
#include "stable.h"

// Constructor
STable::STable()
{	
	this->bm = new BitMap(MAX_SEMAPHORE);
	
	for(int i =0; i < MAX_SEMAPHORE; i++)
	{
		this->semTab[i] = NULL;
	}
}

// Destructor
STable::~STable()
{
	if(this->bm)
	{
		delete this->bm;
	}

	for(int i=0; i < MAX_SEMAPHORE; i++)
	{
		if(this->semTab[i])
		{
			delete this->semTab[i];
		}
	}
	
}

int STable::Create(char *name, int init)
{
	for(int i=0; i<MAX_SEMAPHORE; i++)
	{
		if(bm->Test(i) && strcmp(name, semTab[i]->GetName()) == 0)
			{
				return -1;
			}
	}
	int id = this->FindFreeSlot();
	
	if (id < 0)
	{
		return -1;
	}

	this->semTab[id] = new Sem(name, init);
	return 0;
}

int STable::Wait(char *name)
{
	for(int i =0; i < MAX_SEMAPHORE; i++)
	{
		if(bm->Test(i) && strcmp(name, semTab[i]->GetName()) == 0)
			{
				semTab[i]->wait();
				return 0;
			}
	}
	printf("Semaphore khong ton tai");
	return -1;
}

int STable::Signal(char *name)
{
	for(int i =0; i < MAX_SEMAPHORE; i++)
	{
		if(bm->Test(i) && strcmp(name, semTab[i]->GetName()) == 0)
			{
				semTab[i]->signal();
				return 0;
			}
	}

	printf("Semaphore khong ton tai");
	return -1;
}

int STable::FindFreeSlot()
{
	return this->bm->Find();
}
