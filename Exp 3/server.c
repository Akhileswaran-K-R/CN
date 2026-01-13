#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

void main(){
  struct sockaddr_in serverAddr;
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
  printf("Bind to port number %d\n",ntohs(serverAddr.sin_port));

  int addrSize = sizeof(serverAddr);
  recvfrom(sockfd,buffer,1024,0,(struct sockaddr*)&serverAddr,&addrSize);
  printf("%s\n",buffer);
  strcpy(buffer,"Hello UDP client\n");
  sendto(sockfd,buffer,1024,0,(struct sockaddr*)&serverAddr,sizeof(serverAddr));

  close(sockfd);
}