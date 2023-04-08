//
// Created by os on 11/15/21.
//

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
void solve(){
    int v;
    int sv = 0;
    int p[2];
    int son = 0;
    while(read(0,&v,4)!=0){
        //char buf[100];
        //int cnt = read(0,buf,100);
        //printf("%d: recv %d\n",getpid(),v);

        if(sv == 0){
            sv=v;
            printf("prime %d\n",sv);
        }else if(v%sv){
            //printf("else");
            if(!son){
                pipe(p);
                int r = dup(0);
                close(0);
                dup(p[0]);
                close(p[0]);
                if(fork()){
                    son=1;
                    close(0);
                    dup(r);
                }else{
                    sv=0;
                    close(p[1]);
                    continue;
                }
            }

            write(p[1],(void *)&v,4);

        }
    }

    close(0);
    if(son){
        close(p[1]);
        wait(0);
    }
    exit(0);
}
int main(int argc, char *argv[])
{
    int p[2];
    pipe(p);
    close(0);
    dup(p[0]);
    close(p[0]);
    int i;
    for(i=2;i<=80;i++){
        write(p[1],(void *)&i,4);
    }
    close(p[1]);
    solve();
    exit(0);
    return 0;
}