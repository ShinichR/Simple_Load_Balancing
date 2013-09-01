#include<iostream>
#include "Log/listener.h"

using namespace std; 

#ifndef __LOGGER_H_
#define __LOGGER_H_
class logger{
private:
  string file_name;
public:
  logger();
  logger(string file_name);
  void write_to_file();
  void output_log(char* content);
  void start_listen();
  listener *li;
  ~logger();
};

logger::logger(){
  li = new listener();
  li->create_listen_thread();
}

logger::logger(string file_name){
  this->file_name = file_name;
}

logger::~logger(){
}
void logger::start_listen(){
  li = new listener();
  li->create_listen_thread();
}

void logger::output_log(char* content){
  if(li->no_show()){
    cout<<endl;
    cout<<"---------------------------"<<endl;
    cout<<content<<endl;
    cout<<"---------------------------"<<endl;
  }
}
#endif
