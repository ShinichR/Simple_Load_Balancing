#ifndef __CLIENT_CONFIG_H_
#define __CLIENT_CONFIG_H_
class client_config{
  int num_message;
  int num_pid;
  int id[MAXLINE];
  int usr_id[MAXLINE];

  string *contents;
  int len;
  read_config *re;
public:
  client_config();
  void deal_data();
  ~client_config();
  int get_num_message();
  int  get_num_pid();
  int* get_id();
  int* get_usr_id();
};

client_config::client_config(){
  this->re = new read_config(".clientconf");
  this->re->read_content();
  this->contents = re->get_content();
}

int client_config::get_num_pid(){
   return this->num_pid;
}

void client_config::deal_data(){
  string s = contents[0];
  s = get_substr(s);;
  this->num_message = atoi(s.c_str());
  s = get_substr(contents[1]);

  std::vector<std::string> result = split(s," ");;

  for(int i = 0;i < result.size();i++){
    id[i] = atoi(result[i].c_str());
  }

  s = get_substr(contents[2]);

  result = split(s," ");;

  for(int i = 0;i < result.size();i++){
    usr_id[i] = atoi(result[i].c_str());
  }

  s = get_substr(contents[3]);
  this->num_pid = atoi(s.c_str());
}

int client_config::get_num_message(){
  return this->num_message;
}

int* client_config::get_usr_id(){
  return this->usr_id;
}

int* client_config::get_id(){
  return this->id;
}

#endif
