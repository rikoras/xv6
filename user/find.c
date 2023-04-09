#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"



void solve(char *path, char *target)
{
    int fd = open(path, 0);

    char buf[512];

    memcpy(buf, path, strlen(path));
    char *p = buf + strlen(buf);

    *p++ = '/';

    struct dirent de;

    struct stat st;
    if(fd < 0)
    {
        fprintf(2, "find: Fail to open directory\n");
        exit(1);
    }
    if(fstat(fd, &st) < 0)
    {
        fprintf(2, "find: Fail to get stat\n");
        exit(1);
    }
    if(st.type != T_DIR)
    {
        fprintf(2, "find: %s is not a dirctory!\n", path);
        exit(1);
    }

    if(strlen(path) + 1 + DIRSIZ + 1 > sizeof(buf))
    {
        fprintf(2, "find: Path too long\n");
        exit(1);
    }

    while (read(fd, &de, sizeof(struct dirent)) == sizeof(struct dirent))
    {
        if(de.inum == 0)
            continue;
        if(!strcmp(de.name, ".") || !strcmp(de.name, ".."))
            continue;
        memmove(p, de.name, DIRSIZ);
        p[DIRSIZ] = 0;

        if(stat(buf, &st) < 0)
        {
            fprintf(2, "find: Fail to stat %s\n", buf);
        }

        if(st.type == T_FILE && !strcmp(de.name, target))
        {
            printf("%s\n", buf);
        }

        else if(st.type == T_DIR)
        {
            solve(buf, target);
        }

    }
}

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        fprintf(2, "find: Fail to parse argument\n");
        exit(1);
    }
    solve(argv[1], argv[2]);
    exit(0);
}