#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#define MAXN 80
#define MAXL 15

int nn = 99;            //nn for ninety-nine: end

int my_read(int fd, char* buf, int size)
{
    char bbuf;
    for(int i=0; i<size; i++)
    {
        if(read(fd, &bbuf, 1) == 0)
        {
            fprintf(2, "prime: Failed to read\n");
            return -1;
        }
        buf[i] = bbuf;
    }
    return 0;
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
        write(p[1], (const void*)(&nn), 4);         //pass end signal     
        close(p[1]);                                //close write pipe
        wait(0);
        exit(0);
    }
    else    //for all stages which are not the first
    {
        while(1)
        {
            int pid = -1;
            int first = -1;
            int err = my_read(p[0], (char*)&buf, 4);    //read stage cnt
            if(err == -1)
            {
                fprintf(2, "prime: fail to read\n");
                exit(1);
            }
            int cnt = buf;
            if(cnt < MAXL)
            {
                cnt++;
                if(pipe(pp) < 0)
                {
                    fprintf(2, "prime: Fail to create pipe\n");
                    exit(1);
                }
                pid = fork();
                if(pid < 0)
                {
                    fprintf(2, "prime: Fail to fork\n");
                    exit(1);
                }
                if(pid != 0)
                {
                    close(pp[0]);
                    if(write(pp[1], (const void*)&cnt, 4) < 0)
                    {
                        fprintf(2, "prime: faile to write\n");
                        exit(1);
                    }
                    while(1)
                    {
                        my_read(p[0], (char*)&buf, 4);
                        if(buf == 99)       //received 99 for end
                        {
                            write(pp[1], (char*)&nn, 4);
                            break;
                        }
                        if(first == -1)
                        {
                            //fprintf(1, "prime: %d\n", buf);      //print prime
                            fprintf(1, "p:%d\n", buf);      //print prime
                            first = buf;
                        }
                        else
                        {
                            if(buf % first != 0)
                            {
                                write(pp[1], (const void*)&buf, 4);
                            }
                        }
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
                    if(p[0] == -1)
                    {
                        fprintf(2, "prime: fail to dup\n");
                        close(pp[0]);
                        exit(1);
                    }
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
