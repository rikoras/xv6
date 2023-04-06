#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char **argv)
{
    if(argc <= 1)
    {
        fprintf(1, "sleep: argument needed\n");
        exit(1);
    }
    fprintf(1, "sleep: Start to sleep\n");
    sleep(atoi(argv[1]));
    fprintf(1, "sleep: Wake up!\n");
    exit(0);
}