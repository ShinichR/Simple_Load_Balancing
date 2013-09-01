#ifndef __LB_CONFIG_H_
#define __LB_CONFIG_H_
class lb_config{
  int id;
  int server_udp_port;
  int client_udp_port;

  string *contents;
  int len;
  read_config *re;
public:
  lb_config();
  void deal_data();
  ~lb_config();
  int get_num_pid();
  int get_id();
  int get_server_udb_port();
  int get_client_udb_port();
};

lb_config::lb_config(){
  this->re = new read_config(".lbconf");
  this->re->read_content();
  this->contents = re->get_content();
  this->len = re->get_len();
}

void lb_config::deal_data(){
  string s = contents[0];
  s = get_substr(s);;
  this->id = atoi(s.c_str()); 

  s = get_substr(contents[1]);

  this->server_udp_port = atoi(s.c_str());

  s = get_substr(contents[2]);

  this->client_udp_port = atoi(s.c_str());
}

int lb_config::get_id(){
  return this->id;
}

int lb_config::get_server_udb_port(){
  return this->server_udp_port;
}

int lb_config::get_client_udb_port(){
  return this->client_udp_port;
}




lb_config::~lb_config(){
  delete contents;
  delete re;
}

#endif
