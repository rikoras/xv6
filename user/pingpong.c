#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main()
{
    int pipe_ping[2];
    int pipe_pong[2];
    int cnt = 100;
    char buf[20];
    char buff_L2[20];
    int p = 0;
    if(pipe(pipe_ping) < 0)
    {
        fprintf(1, "pingpong: Pipe failed!\n");
        exit(1);
    }
    if(pipe(pipe_pong) < 0)
    {
        fprintf(1, "pingpong: Pipe failed!\n");
        exit(1);
    }
    int pid = fork();
    if(pid < 0)
    {
        fprintf(1, "pingpong: Fork failed!\n");
        exit(1);
    }
    else if(pid == 0)
    {
        fprintf(pipe_ping[1], "Ping\n");
        while (cnt > 0)
        {
            if(read(pipe_pong[0], buf, sizeof(buf)) == 0)
            {
                fprintf(1, "pingpong: Child read failed\n");
                continue;
            }
            strcpy(buff_L2+p, buf);
            p += strlen(buf);
            memset(buf, 0, sizeof(buf));
            fprintf(1, "pingpong: Child read %s\n", buff_L2);
            if(!strcmp(buff_L2, "Pong\n"))
            {
                fprintf(1, "pingpong: Child ping\n");
                fprintf(pipe_ping[1], "Ping\n");
                cnt --;
                p = 0;
            }
        }
    }
    else
    {
        while (cnt > 0)
        {
            if(read(pipe_ping[0], buf, sizeof(buf)) == 0)
            {
                fprintf(1, "pingpong: Parent read failed\n");
                continue;
            }
            //fprintf(1, "read %s\n", buf);
            strcpy(buff_L2+p, buf);
            p += strlen(buf);
            memset(buf, 0, sizeof(buf));
            fprintf(1, "pingpong: Parent read %s\n", buf);
            if(!strcmp(buff_L2, "Ping\n"))
            {
                fprintf(1, "pingpong: Parent pong\n");
                fprintf(pipe_pong[1], "Pong\n");
                cnt --;
                p = 0;
            }
        }
    }
    exit(0);
    
}