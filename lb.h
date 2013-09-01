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
#include    <unistd.h>
#include	<sys/time.h>	/* timeval{} for select() */
using namespace std;

#define pb     push_back
#define MAXLINE 200
#define MAX_SERVER 10
#define MAX_CLIENT 1
#define	SA	struct sockaddr

#define TIMEREQUEST  0
#define TIMEANSWER   1
#define HEARTREQUEST 2
#define HEARTANSWER  3
#define LBID         11000
#define client_udp_port 9999
#define server_udp_port 8888
#define BUFSIZE      7000


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
