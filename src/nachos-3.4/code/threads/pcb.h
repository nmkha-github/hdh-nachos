#ifndef PCB_H
#define PCB_H

#include "thread.h"
#include "synch.h"

class PCB
{
private:
    Semaphore *joinsem; // semaphore cho quá trình join
    Semaphore *exitsem; // semaphore cho quá trình exit
    Semaphore *multex;  // semaphore cho quá trình truy xuất đọc quyền
    int exitcode;
    int numwait;    // số tiến trình đã join
    char name[255]; // Tên tiến trình
    Thread *thread;

public:
    int parentID; // ID của tiến trình cha
    int pid;      // ID của process
    PCB();
    PCB(int id); // contructor
    ~PCB();      // deconstructor
    // Nạp vào chương trình
    int Exec(char *filename, int pid);
    int GetID();
    int GetNumWait();
    void JoinWait();
    void ExitWait();
    void JoinRelease();
    void ExitRelease();
    void IncNumWait();
    void DecNumWait();

    void SetExitCode(int ec);
    int GetExitCode();

    void SetFileName(char *fn); // Set tên cho tiến trình
    char *GetFileName();        // Get tên của tiến trình
};

#endif
