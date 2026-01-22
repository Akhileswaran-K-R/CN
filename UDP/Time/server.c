#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<time.h>

void main(){
  struct sockaddr_in serverAddr,clientAddr;
  char buffer[1024];

  int sockfd = socket(AF_INET,SOCK_DGRAM,0);
  if(sockfd == -1){
    printf("\nError creating socket\n");
    exit(-1);
  }
  printf("Server socket created successfully\n");

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(6666);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if(bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) == -1){
    printf("\nError binding socket\n");
    exit(-1);
  }
  printf("Bind to port number %d\n\n",ntohs(serverAddr.sin_port));

  int addrSize = sizeof(clientAddr);
  while(1){
    recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&clientAddr,&addrSize);

    pid_t pid = fork();
    if(pid < 0){
      printf("\nFork error\n");
    }else if(pid == 0){
      time_t t = time(NULL);
      char *timeStr = ctime(&t);

      sendto(sockfd,timeStr,strlen(timeStr)+1,0,(struct sockaddr*)&clientAddr,sizeof(clientAddr));
      exit(0);
    }else{
      printf("Time retrieved from child\n");
    }
  }

  close(sockfd);
}