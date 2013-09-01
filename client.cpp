#include    <iostream>
#include	<sys/types.h>	/* basic system data types */
#include	<sys/socket.h>	/* basic socket definitions */
#include    <sys/types.h>
#include    <sys/wait.h>
#include	<string.h>	/* basic socket definitions */
#include    <arpa/inet.h>
#include    <stdio.h>
#include	<stdlib.h>
#include    <vector>

#include        "read_config.h"
#include        "common.h"
#include        "server_config.h"
#include        "client_config.h"
#include        "lbconfig.h"
using namespace std;



#define pb push_back
#define	SA	struct sockaddr

static int  message_num;
static int* id;
static int* usr_id;
static int  pid_num;
static int  client_udp_port;


static int msg_count=0, wrong_count=0,true_count=0;

void
dg_cli(FILE *fp, int sockfd, SA *pservaddr, socklen_t servlen,int clientnumber)
{
    int		size;
    char		sendline[MAXLINE], recvline[MAXLINE + 1];
    ssize_t		n;
    socklen_t	len;

    fd_set readfds;
    struct timeval tv;
    char buf1[MAXLINE], buf2[MAXLINE];
    size = 70000;

    setsockopt(sockfd, SOL_SOCKET, SO_SNDBUF, &size, sizeof(size));

    setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

    printf("the %d is client is requesting time.",clientnumber);

    int i=0;
    while(1&&i<message_num){
        FD_ZERO(&readfds);

        // add our descriptors to the set
        FD_SET(sockfd, &readfds);
        //  FD_SET(s2, &readfds);

        // wait until either socket has data ready to be recv()d (timeout 10.5 secs)
        tv.tv_sec = 10;
        tv.tv_usec = 500000;
        t_msg msg;
        msg.src_id = id[clientnumber];
        msg.usr_id = usr_id[i];
        if(i%2)
        msg.dst_id = LBID;
        else msg.dst_id = i;
        msg.msg_type = TIMEREQUEST;

        printf("befor send message dist_id:%d  the %d'smessage\n",msg.dst_id,i);

        sendto(sockfd, &msg, MAXLINE, 0, pservaddr, servlen);
        printf("the message has sendend\n");

        int rv = select(sockfd+1, &readfds, NULL, NULL, &tv);


       /* if (rv == -1) {
            perror("select"); // error occurred in select()
        } else if (rv == 0) {
            printf("Timeout occurred!  No data after 10.5 seconds.\n");
        } */
        if(rv > 0){
                recvfrom(sockfd, &msg, MAXLINE,0,pservaddr, &servlen);
                msg_count++;
                if(strcmp(msg.data,"abondon")==0)wrong_count++;
                else true_count++;

                cout<<"client "<<clientnumber<<"--"<<i<<"s request"<<" Accepted Time:"<<msg.data<<endl;
                i++;
        }
        else{
        	i++;
        }
    }

    exit(1);
 // printf("received %d bytes\n", n);
}

void sig_chld(int signo){
    pid_t pid;
    int stat;
    while((pid = waitpid(-1,&stat,WNOHANG))>0){
        printf("child %d terminated\n",pid);
    }
    printf("server all stop\n");
    cout<<"msg_count= "<<msg_count<<" ,wrong_count= "<<wrong_count<<" ,true_count"<<true_count<<endl;
}

int main(int argc,char **argv){


//Get the conf
  client_config *client = new client_config();
  client->deal_data();

  //Set the message num
  message_num = client->get_num_message();
  //Set the usr_id
  usr_id = client->get_usr_id();
  //Set the progress id
  id = client->get_id();
  pid_num = client->get_num_pid();

  lb_config *lb = new lb_config();
  lb->deal_data();
  //Set the client port
  client_udp_port = lb->get_client_udb_port();

  cout<<"message_num===="<<message_num<<"pid_num==="<<pid_num<<"client_udp_port"<<client_udp_port<<endl;


    int	 sockfd,forid;
    struct sockaddr_in	servaddr,clientaddr;


 // bzero(&clientaddr,sizeof (clientaddr));
 // clientaddr.sin_family = AF_INET;
 // clientaddr.sin_port = htons(9999);

    //freopen ("myfile.txt","w",stdout);

    signal(SIGCHLD, sig_chld);

    for(int i = 0; i<pid_num; i++){
        if(forid = fork() > 0){
            bzero(&servaddr, sizeof(servaddr));
            servaddr.sin_family = AF_INET;
            servaddr.sin_port = htons(client_udp_port);

            inet_pton(AF_INET, "127.0.0.1", &servaddr.sin_addr);
         //vec_pid.pb(forid);
            sockfd = socket(AF_INET, SOCK_DGRAM, 0);
         //printf("sockfd=%d",sockfd);
            dg_cli(stdin, sockfd, (SA *) &servaddr, sizeof(servaddr),i);
        }
        close(forid);
    }
    exit(0);
}
