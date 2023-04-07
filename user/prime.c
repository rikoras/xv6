#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define MAXN 35
#define MAXL 8

int nn = 99;

void my_read(int fd, char* buf, int size)
{
    char bbuf;
    for(int i=0; i<size; i++)
    {
        if(read(fd, &bbuf, 1) == 0)
        {
            fprintf(2, "prime: Failed to read\n");
            exit(1);
        }
        buf[i] = bbuf;
    }
}

int main()
{
    int p[2];
    int pp[2];
    int buf = -1;
    if(pipe(p) < 0)
    {
        fprintf(2, "prime: pipe failed\n");
        exit(1);
    }
    int pid = fork();
    if(pid < 0)
    {
        fprintf(2, "prime: fail to fork\n");
        exit(1);
    }
    if(pid != 0)
    {
        close(p[0]);
        int cnt_1 = 1;
        write(p[1], (const void*)&cnt_1, 4);
        for(int i=3; i<=MAXN; i+=2)
        {
            write(p[1], (const void*)(&i), 4);     //pass all the odd nums
        }
        write(p[1], (const void*)(&nn), 4);     //pass all the odd nums     
        close(p[1]);        //close write pipe
        wait(0);
        exit(0);
    }
    else    //for all stages which are not the first
    {
        while(1)
        {
            int pid = -1;
            my_read(p[0], (char*)&buf, 4);
            int cnt = buf;
            fprintf(1, "prime: This Proc have a count of %d\n", cnt);
            if(cnt < MAXL)
            {
                cnt++;
                if(pipe(pp) < 0)
                {
                    fprintf(2, "prime: Fail to create pipe\n");
                }
                pid = fork();
                if(pid < 0)
                {
                    fprintf(1, "prime: Fail to fork\n");
                }
                if(pid != 0)
                {
                    close(pp[0]);
                    if(write(pp[1], (const void*)&cnt, 4) < 0)
                    {
                        fprintf(2, "prime: write failed\n");
                    }
                    close(pp[1]);
                    close(p[0]);
                    wait(0);
                    exit(0);
                }
                else
                {
                    close(pp[1]);
                    p[0] = dup(pp[0]);
                    close(pp[0]);
                }
            }
            else
            {
                exit(0);
            }
        }
    }
    exit(0);
}
