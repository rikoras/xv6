#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

int main(int argc, char* argv[])
{
    /*if(argc == 1)
    {
        fprintf(2, "xargs: Argument required\n");
        exit(1);
    }*/
    //char buf[512];
    //read(0, buf, 100);
    //printf("%s", buf);
    char *args[] = {".", 0};
    
    exec("ls", args);
    exit(0);
}

