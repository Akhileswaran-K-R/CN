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
  printf("Client socket created successfully\n");

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(6666);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  strcpy(buffer,"Hello UDP server\n");
  sendto(sockfd,buffer,strlen(buffer)+1,0,(struct sockaddr*)&serverAddr,sizeof(serverAddr));

  int addrSize = sizeof(serverAddr);
  recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&serverAddr,&addrSize);
  printf("%s\n",buffer);
  
  close(sockfd);
}