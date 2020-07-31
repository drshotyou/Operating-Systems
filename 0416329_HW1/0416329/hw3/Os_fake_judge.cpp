#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <cassert>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

int main (int argc , char **argv){
    int shmid = 0;
    int *shm;
    srand (time(NULL));
    try{
        key_t key_value =  ftok("123.txt", 123);
        cout << key_value << endl;
        shmid = shmget(key_value, sizeof(int), IPC_CREAT|0666);
        shm = (int*)shmat(shmid, NULL, 0);
        string s ;
        int ti = 100;
        int score = 0;
        while(ti--){
            int temp = rand() % 3;
            *shm = temp;
            int ans;
            cout << "OK" <<endl;
            //cout.flush();
            string opponent_option ;
            cin >> opponent_option;
            //cerr << "JudgeSide: "<< ti << " " << *shm << opponent_option << endl;
            
            if(temp==0)  s="Paper";
            else if(temp==1)  s="Scissor";
            else if(temp==2)  s="Rock";
            //cout << *shm << s << endl;
            cout << temp << endl;

            //cout.flush();
            if(opponent_option == "Scissor")
            {
              ans=1;//paper
            } else if(opponent_option == "Paper")
            {
              ans=0;//scissor
            }else if(opponent_option == "Rock")
            {
              ans=2;//rock
            }
            //ans = rand() % 3;
            //if(ans==0)  s="Paper";
            //else if(ans==1)  s="Scissor";
            //else if(ans==2)  s="Rock";
            //cout << *shm << s << endl;
            //cout << s << endl;
            if((*shm==0 && ans==1) ||
               (*shm==1 && ans==2) ||
               (*shm==2 && ans==0))
               {
                 score++;
               }
            else if(*shm==ans)
            {

            }
            else if ((*shm==1 && ans==0) ||
                     (*shm==2 && ans==1) ||
                     (*shm==0 && ans==2))
            {
              score--;
            }
            //cout << score << endl;
        }
        cerr << score << endl;
        shmdt(shm);
        shmctl(shmid,IPC_RMID,0);
    }
    catch(...){
        cerr << 0 <<endl;
        if(shm != 0){
            shmdt(shm);
            shmctl(shmid,IPC_RMID,0);
        }
    }
}
