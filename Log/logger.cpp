#include<iostream>
#include <stdio.h>
#include <termios.h> /* for tcxxxattr, ECHO, etc */
#include <unistd.h> /* for STDIN_FILENO */
#include <pthread.h> /* for STDIN_FILENO */
#include <stdlib.h> /* for STDIN_FILENO */


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

#ifndef __LOGGER_H_
#define __LOGGER_H_
//Logger class include Logger and listener
class Logger{
private:
  string file_name;
  int* signal;
public:
  Logger();
  Logger(string file_name);
  static void write_to_file(string file_name,char *content);
  static void* listen_Output(void *signal);
  void create_listen_thread(int *signal);
  void output_log(int content);
  ~Logger();
};

Logger::Logger(){
}

Logger::~Logger(){
  delete signal;
}

Logger::Logger(string file_name){
  this->file_name = file_name;
}

void Logger::output_log(int content){
  printf("%d\n",content);
}

//Create the listener thread
void Logger::create_listen_thread(int *signal){
  this->signal = signal;
  pthread_t id;

  pthread_attr_t thread_attr;
  pthread_attr_init (&thread_attr);
  pthread_attr_setdetachstate (&thread_attr, PTHREAD_CREATE_DETACHED);

  int n = pthread_create(&id,&thread_attr,&this->listen_Output,this->signal);
}

//Listen the key press to show or close lb message
void* Logger::listen_Output(void* signal){
  for(;;){
    int c = getch();
    cout<<"C==="<<c<<endl;
    sleep(100);
    int* i = (int *)signal;
    if (c == 84){
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
#endif

int main(){
  int i;
  Logger lo;
  lo.create_listen_thread(&i);
  while(true){
    cout<<"Waiting ~ ~ ~"<<endl;
  }
}
