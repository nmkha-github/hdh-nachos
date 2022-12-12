#include "synch.h"
#include "stable.h"

STable::STable()
{
  bm = new BitMap(MAX_SEMAPHORE);
  for (int i = 0; i < MAX_SEMAPHORE; i++)
  {
    semTab[i] = NULL;
  }
}

STable::~STable()
{
  for (int i = 0; i < MAX_SEMAPHORE; i++)
  {
    if (semTab[i])
    {
      delete semTab[i];
    }
  }
  if (bm)
  {
    delete bm;
  }
}

int STable::Create(char *name, int init)
{
  for (int i = 0; i < MAX_SEMAPHORE; i++)
  {
    if (bm->Test(i))
    {
      if (strcmp(name, semTab[i]->GetName()) == 0)
      {
        printf("\nSemaphore nay da ton tai\n");
        return -1;
      }
    }
  }

  int pid = FindFreeSlot();
  if (pid < 0)
  {
    printf("\nKhong con slot trong\n");
    return -1;
  }

  semTab[pid] = new Sem(name, init);
  return 0;
}

int STable::Wait(char *name)
{
  for (int i = 0; i < MAX_SEMAPHORE; i++)
  {
    if (bm->Test(i))
    {
      if (strcmp(name, semTab[i]->GetName()) == 0)
      {
        semTab[i]->wait();
        return 0;
      }
    }
  }
  printf("\nSemaphore khong ton tai!\n");
  return -1;
}

int STable::Signal(char *name)
{
  for (int i = 0; i < MAX_SEMAPHORE; i++)
  {
    if (bm->Test(i))
    {
      if (strcmp(name, semTab[i]->GetName()) == 0)
      {
        semTab[i]->signal();
        return 0;
      }
    }
  }
  printf("\nSemaphore khong ton tai!\n");
  return -1;
}

int STable::FindFreeSlot()
{
  return bm->Find();
}
