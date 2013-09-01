
#include    <iostream>
#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#include	<string.h>	/* basic socket definitions */
#include    <arpa/inet.h>
#include    <stdio.h>
#include	<stdlib.h>
#include	<signal.h>
#include    <map>
#include    <cstring>
#include    <vector>
#include    <pthread.h>
#include    <unistd.h>
#include	<sys/time.h>	/* timeval{} for select() */
using namespace std;

#define pb     push_back
#define MAXLINE 200
#define	SA	struct sockaddr

#define TIMEREQUEST  0
#define TIMEANSWER   1
#define HEARTREQUEST 2
#define HEARTANSWER  3
#define LBID         11000
#define BUFSIZE      7000




typedef struct{

    unsigned msg_count;//接收消息条数

    unsigned msg_true;//正确消息

    unsigned msg_wrong;//错误消息

    unsigned msg_answer;//应答消息

}Statistics;

typedef struct
{
    /* 消息的发送进程是谁，就填谁的id */
    unsigned src_id;
/* 消息的接收进程是谁，就填谁的id */
    unsigned dst_id;
/* 发送“时间请求”消息时填写，
回复“时间应答”消息时，其值要与请求消息保持一致。 */
    unsigned usr_id;
/* 消息类型：0, 时间请求；1, 时间答应；2, 心跳请求；3, 心跳应答 */
    unsigned msg_type;
/* 服务端回复“时间应答”消息时，
在data中填入当前时间的字符串，形式如“2013-06-20 13:56:28”即可  */
    char data[32];
} t_msg;


const string get_substr(string str){
  int loc = str.find("=",0);
  str = str.substr(loc+1,str.length());
  return str;
}


std::vector<std::string> split(std::string str,std::string pattern)
{
  std::string::size_type pos;
  std::vector<std::string> result;
  str+=pattern;
  int size=str.size();

  for(int i=0; i<size; i++)
    {
      pos=str.find(pattern,i);
      if(pos<size)
	{
	  std::string s=str.substr(i,pos-i);
	  result.push_back(s);
	  i=pos+pattern.size()-1;
	}
    }
  return result;
}

