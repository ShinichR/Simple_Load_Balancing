#include<iostream>
#include <stdio.h>
#include <termios.h> /* for tcxxxattr, ECHO, etc */
#include <unistd.h> /* for STDIN_FILENO */
#include <pthread.h> /* for STDIN_FILENO */
#include <stdlib.h> /* for STDIN_FILENO */
#include <string.h> /* for STDIN_FILENO */


using namespace std; 
#ifndef __GETCH__
#define __GETCH__
/*simulate windows' getch() */
int getch (void){
  int ch;
  struct termios oldt, newt;

  // get terminal input's attribute
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;

  //set termios' local mode
  newt.c_lflag &= ~(ECHO|ICANON);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  //read character from terminal input
  ch = getchar();

  //recover terminal's attribute
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  return ch;
}
#endif



class listener{
private:
  char identifier;
  int signal;
  pthread_t pthread_id;
  pthread_attr_t pthread_attr;
public:
  listener(char identifier);
  listener();
  void create_listen_thread();
  static void* listen_Output(void *signal);
  bool no_show();
};
listener::listener(){
}
listener::listener(char identifier){
  this->identifier = identifier;
  signal = 1;
}
bool listener::no_show(){
  if(signal == 0){
    return false;
  }
  return true;
}
void listener::create_listen_thread(){
  pthread_attr_init (&this->pthread_attr);
  pthread_attr_setdetachstate (&this->pthread_attr, PTHREAD_CREATE_DETACHED);

  int err_thread = pthread_create(&pthread_id,&pthread_attr,&this->listen_Output,&this->signal);
  if(err_thread != 0){
    cout<<strerror(err_thread)<<endl;
  }
}

void* listener::listen_Output(void *signal){
  while(true){
    int c = getch();
    int* i = (int *)signal;
    if(c == 84 || c == 116){
      int tmp = *i;
      if (tmp == 0){
	cout<<"T is pressed will  show the message"<<endl;
      }else{
	cout<<"T is pressed will  close the message"<<endl;
      }
      tmp == 0?tmp = 1:tmp = 0;
      *i = tmp;
    }
  }
}

int main(){
  listener li;
  li.create_listen_thread();
  while(true){
    if(li.no_show()){
      cout<<"Don't show"<<endl;
    }
      cout<<"show"<<endl;
  }
  return 0;
}
