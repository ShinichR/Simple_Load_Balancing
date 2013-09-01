#include        "read_config.h"
#include        "common.h"
#include        "server_config.h"
#include        "client_config.h"
#include        "logger.h"
#include        <signal.h>
#include        <unistd.h>
#include        <sys/ipc.h>
#include        <sys/shm.h>
int count = 0;

static int  num_pid;
static int* port;

static int* ServerID;
static Statistics *sta;
int *SIGNAL;
char tmp_str[2000];

void output_log(char *content){
  if(*SIGNAL == 1){
    cout<<"-------------------------------------------------------------------------------"<<endl;
    cout<<content<<endl;
    cout<<"-------------------------------------------------------------------------------"<<endl;
  }
}

void dg_echo(int sockfd, SA *pcliaddr, socklen_t clilen,int servernumber)
{
  socklen_t	len;
  char		mesg[MAXLINE];
  int size = 7000;
  struct sockaddr_in cliaddr;
  bzero(&cliaddr, sizeof(cliaddr));
  time_t timer =time(NULL);

  setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));

  for ( ; ; ) {

    len = clilen;
    t_msg clientmsg;

    recvfrom(sockfd, &clientmsg, MAXLINE, 0, pcliaddr, &len);
    sta->msg_count++;


    sprintf(tmp_str,"server id = %d is dealing the request\n",ServerID[servernumber]);
    output_log(tmp_str);
    srand((unsigned)time(0));
    int ran_num = (rand() % (30))+ 1;

    sprintf(tmp_str,"server will sleep %ds to test choose another server\nServer id %d is accepted.",ran_num,ServerID[servernumber]);
    output_log(tmp_str);

    sprintf(tmp_str,"client id:%d message type:%d client destination id:%d ",clientmsg.dst_id,clientmsg.msg_type,clientmsg.src_id,ServerID[servernumber]);
    output_log(tmp_str);

    if(clientmsg.dst_id != ServerID[servernumber]){
      sta->msg_wrong++;
      cout<<"BL msg abondon "<<endl;
    }
    else {
      sta->msg_true++;
      //sleep(ran_num);
      strcpy(clientmsg.data,ctime(&timer));
      int n = sendto(sockfd, &clientmsg, MAXLINE, 0, pcliaddr, len);
      sta->msg_answer++;
      if (n < 0){
	sprintf(tmp_str,"sendto %d Error",n);
	output_log(tmp_str);
      }
      count++;
    }
  }
}

void when_alarm(int signo)
{
  sprintf(tmp_str,"服务器接收到 %d 条消息,正确的 %d 条，错误的 %d 条,应答的 %d 条信息！\n",sta->msg_count,sta->msg_true,sta->msg_wrong,sta->msg_answer);
  output_log(tmp_str);
}


int main(int argc,char **argv){
  int					sockfd,forid;
  int x, shmid,shmid_log;
  listener li;

  //Listen the logger
  //---------------
  if((shmid_log = shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT|0666))<0)
    printf("shmget error\n"),exit(1);

  if((SIGNAL = (int*)shmat(shmid_log,0,0)) == (int *)-1)
    printf("shmat error\n"),exit(1);
  *SIGNAL = 1;

  li.create_listen_thread(SIGNAL);
  //---------------

  struct sockaddr_in	servaddr, cliaddr;

  //Statistics message
  //--------------------
  if((shmid = shmget(IPC_PRIVATE,sizeof(int),IPC_CREAT|0666))<0)
    printf("shmget error\n"),exit(1);

  if((sta = (Statistics*)shmat(shmid,0,0)) == (Statistics *)-1)
    printf("shmat error\n"),exit(1);
  //-------------------


  sta->msg_answer=sta->msg_count=sta->msg_true=sta->msg_wrong=0;

  server_config *se = new server_config();
  se->deal_data();

  num_pid = se->get_num_pid();
  port = se->get_port();

  ServerID = se->get_id();

  signal(SIGALRM,when_alarm);

  for(int i=0; i<num_pid; i++){
    if(forid=fork()>0){
      bzero(&servaddr, sizeof(servaddr));
      servaddr.sin_family      = AF_INET;
      servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
      servaddr.sin_port        = htons(port[i]);
      sockfd = socket(AF_INET, SOCK_DGRAM, 0);
      sprintf(tmp_str,"the %d server.it's port:%d\n",i,port[i]);
      output_log(tmp_str);
      bind(sockfd, (SA *) &servaddr, sizeof(servaddr));
      dg_echo(sockfd, (SA *) &cliaddr, sizeof(cliaddr),i);
    }
  }
  while(1){
    alarm(5);
    pause();

  }
  return 0;
}
