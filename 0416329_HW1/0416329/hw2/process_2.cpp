#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <iostream>
#include <errno.h>

using namespace std;

int main(int argc ,char* argv[]){
    int pidx=0,pidy=0,pidz=0;
    printf("create main process %d\n",getpid());
    printf("process %d create process %d\n",getpid(),pidx=fork());
    printf("process %d create process %d\n",getpid(),pidy=fork());
    printf("process %d create process %d\n",getpid(),pidz=fork());

    /*
        your code
    */

    pidx=0;pidy=0;pidz=0;
    for (;;) {
        pid_t waiting_child;
        waiting_child = wait(NULL);

        if (pidx == 0 && waiting_child != -1) {
            pidx = waiting_child;
        } else {
            if (pidy == 0 && waiting_child != -1) {
                pidy = waiting_child;
            }
            else {
                if (pidz == 0 && waiting_child != -1) {
                    pidz = waiting_child;
                }
            }
        }

        if (waiting_child == -1) {
            if (errno == ECHILD) {
            break;
            } else {
            printf("wait");
            }
        }
    }
    printf("process %d exit its child process %d %d %d\n",getpid(),pidx,pidy,pidz);
}
