// pcb.h 
//  Process Control Block
//	Save all information to control the process
// All rights reserved.

#ifndef PCB_H
#define PCB_H

#include "thread.h"
#include "synch.h"

// Process Control Block
class PCB
{
private:
    Semaphore* joinsem;         
    Semaphore* exitsem;         
    Semaphore* multex;          // semaphore dành cho truy xuất độc quyền  

    int exitcode;		
    int numwait;                // số tiến trình đã join

    char FileName[255];          // Tên tiến trình

    Thread* thread;             
public:
    int parentID;               // ID tiến trình cha
    
    char boolBG;                
    
    PCB(int = 0);               
    ~PCB();                     

    int Exec(char*,int);        
    int GetID();                // Trả về ProcessID của tiến trình gọi thực hiện
    int GetNumWait();           // Trả về số lượng tiến trình chờ


    void JoinWait();            // 1. Tiến trình cha đợi tiến trình con kết thúc
                        
    void ExitWait();             // 4. Tiến trình con kết thúc

    void JoinRelease();         // 2. Báo cho tiến trình cha thực thi tiếp
    void ExitRelease();         // 3. Cho phép tiến trình con kết thúc

    void IncNumWait();          // Tăng số tiến trình chờ
    void DecNumWait();          // Giảm số tiến trình chờ

    void SetExitCode(int);      // Đặt exitcode của tiến trình
    int GetExitCode();          // Trả về exitcode

    void SetFileName(char*);    // Hàm set tên cho tiến trình
    char* GetFileName();        // Hàm get tên tiến trình

};

#endif // PCB_H
