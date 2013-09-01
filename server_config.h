#ifndef __SERVER_CONFIG_H_
#define __SERVER_CONFIG_H_
class server_config{
  int num_pid;
  int id[MAXLINE];
  int port[MAXLINE];

  string *contents;
  int len;
  read_config *re;
public:
  server_config();
  void deal_data();
  ~server_config();
  int get_num_pid();
  int* get_id();
  int* get_port();
};
server_config::~server_config(){
  delete contents;
  delete re;
}

server_config::server_config(){
  this->re = new read_config(".serverconf");
  this->re->read_content();
  this->contents = re->get_content();
  this->len = re->get_len();
}

void server_config::deal_data(){
  string s = contents[0];
  s = get_substr(s);;
  this->num_pid = atoi(s.c_str());
  s = get_substr(contents[1]);
  std::vector<std::string> result = split(s," ");;

  for(int i = 0;i < result.size();i++){
    id[i] = atoi(result[i].c_str());
  } 

  s = get_substr(contents[2]);

  result = split(s," ");;

  for(int i = 0;i < result.size();i++){
    port[i] = atoi(result[i].c_str());
  } 

}

int server_config::get_num_pid(){
  return this->num_pid;
}

int* server_config::get_port(){
  return this->port;
}

int* server_config::get_id(){
  return this->id;
}

#endif
