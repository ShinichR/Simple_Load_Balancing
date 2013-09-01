#include<iostream>
#include<fstream>

using namespace std;
#define MAXLINES 2000
class read_config{
  string path;
  string contents[MAXLINES];
  int len;
public:
  read_config(string path);
  void output_path();
  void read_content();
  string* get_content();
  int get_len();
  ~read_config();
};

read_config::read_config(string path){
  this->path = path;
}

void read_config::output_path(){
  cout<<this->path<<endl;
}

void read_config::read_content(){
  string s;
  const char *file_path = this->path.c_str();
  ifstream in(file_path);
  int i = 0;
  while(getline(in,s)){
    contents[i] = s;
    i++;
  }
  this->len=i;
}

string* read_config::get_content(){
  return this->contents;
}
int read_config::get_len(){
  return this->len;
}


read_config::~read_config(){
}

