#include<bits/stdc++.h>
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

int main(){
    int shmid = 0;
    int * shm;
    srand(time(NULL));
    try{
        string s;
        int key;
        cin >> key;
        shmid = shmget(key,sizeof(int),IPC_CREAT|0666);
        shm = (int*)shmat(shmid,NULL,0);
        string temp;
        int temp2;
        for(int i=0;i<100;i++){

            //cerr << "TaskSide: "<< i << " " << *shm << endl;
            //cout << shm << endl;
            //*shm = rand() % 3;
            //cout << (int)*shm << endl;
            cin >> temp;
            if(*shm==0)//paper
            {
              s = "Scissor";
              //cout << "Paper " << s << endl;
            }else if(*shm==1)//Scissor
            {
              s = "Rock";
              //cout << "Scissor " << s << endl;
            }else if(*shm==2)//Rock
            {
              s = "Paper";
              //cout << "rock " << s << endl;
            }
            cout<<s<<endl;
            cout.flush();
            cin >> temp2;
        }
      }
      catch(...){
        cerr << 0 << endl;
        if(shm !=0)
        {
          shmdt(shm);
          shmctl(shmid,IPC_RMID,0);
        }
      }

}
