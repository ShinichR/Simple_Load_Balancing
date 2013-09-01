#include<iostream>
#include <stdio.h>
#include <termios.h> 
#include <unistd.h> 
#include <pthread.h> 
#include <stdlib.h> 
#include <string.h> 
using namespace std; 

#ifndef __GETCH__
#define __GETCH__

int getch (void){
  int ch;
  struct termios oldt, newt;

  
  tcgetattr(STDIN_FILENO, &oldt);
  newt = oldt;

  
  newt.c_lflag &= ~(ECHO|ICANON);
  tcsetattr(STDIN_FILENO, TCSANOW, &newt);

  
  ch = getchar();

  
  tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

  return ch;
}
#endif



#ifndef __LISTENER_H_
#define __LISTENER_H_

class listener{
private:
  char identifier;
  int signal;
  pthread_t pthread_id;
  pthread_attr_t pthread_attr;
  Statistics *statistics;
public:
  listener(char identifier);
  listener();
  void create_listen_thread();
  void create_listen_thread(Statistics *statistics);
  void create_listen_thread(int *si);
  static void* listen_Output(void *signal);
  static void* listen_lb_Output(void *statistics);
  bool no_show();
};

listener::listener(){
  signal = 1;
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

void listener::create_listen_thread(int *si){
  pthread_attr_init (&this->pthread_attr);
  pthread_attr_setdetachstate (&this->pthread_attr, PTHREAD_CREATE_DETACHED);

  int err_thread = pthread_create(&pthread_id,&pthread_attr,&this->listen_Output,si);

  if(err_thread != 0){
    cout<<strerror(err_thread)<<endl;
  }
}

void listener::create_listen_thread(Statistics *statistics){
  pthread_attr_init (&this->pthread_attr);
  pthread_attr_setdetachstate (&this->pthread_attr, PTHREAD_CREATE_DETACHED);

  int err_thread = pthread_create(&pthread_id,&pthread_attr,&this->listen_lb_Output,statistics);

  if(err_thread != 0){
    cout<<strerror(err_thread)<<endl;
  }
}


void* listener::listen_Output(void *signal){
  int* i = (int *)signal;
  while(true){
    int c = getch();
    if(c == 84 || c == 116){
      int tmp = *i;
      if (tmp == 0){
	cout<<"T is pressed will show the message"<<endl;
      }else{
	cout<<"T is pressed will close the message"<<endl;
      }
      tmp == 0?tmp = 1:tmp = 0;
      *i = tmp;
    }
  }
}
void* listener::listen_lb_Output(void *statistics){
  Statistics* i = (Statistics *)statistics;
  while(true){
    int c = getch();
    if(c == 84 || c == 116){
      Statistics tmp = *i;
      cout<<"Statistics------------"<<endl;
    }
  }
}



#endif

