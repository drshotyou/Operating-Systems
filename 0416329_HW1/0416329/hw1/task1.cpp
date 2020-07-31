#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>

using namespace std;
int main () {
    pid_t pid;

    for(int i=1;i<=10;i++)
    {

      cout << "process pid " << getpid() << " create" << endl;
      pid = fork();
      if(pid)
      {
        wait(NULL);
        cout << "process pid " << getpid() << " exit" << endl;
        break;
      }
      
      //cout << getpid() << endl;
    }


  return 0;
}

/*
pid_t pid;

for(int i=1;i<=10;i++)
{

  cout << "process pid: " << getppid() << " for child id:" << getpid() << endl;
  pid = fork();
  if(pid)
  {
    break;
  }
  //cout << getpid() << endl;
}
*/
