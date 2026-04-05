#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>
#include<string.h>
#include <stdatomic.h>

void reverse(char buffer[]){
  int n = strlen(buffer);
  for(int i=0;i<n/2;i++){
    char temp = buffer[i];
    buffer[i] = buffer[n-i-1];
    buffer[n-i-1] = temp;
  }
}

void *handler(void *args){
  int *sockfd = args;
  char buffer[1024];

  while(1){
    recv(*sockfd,buffer,sizeof(buffer),0);
    if(strcmp(buffer,"Reverse") == 0){
      printf("Client: %s\n",buffer);
      reverse(buffer);
      send(*sockfd,buffer,strlen(buffer)+1,0);
    }else if(strcmp(buffer,"exit") == 0){
      break;
    }else{
      printf("Client: %s\n",buffer);
    }
  }

  close(*sockfd);
  pthread_detach(pthread_self());
  return NULL;
}

void main(){
  struct sockaddr_in serverAddr,clientAddr;
  char buffer[1024];
  pthread_t tid;

  int serverSocket = socket(AF_INET,SOCK_STREAM,0);
  printf("Server socket created successfully\n");

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(4444);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  bind(serverSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
  printf("Bind to port number %d\n",ntohs(serverAddr.sin_port));

  listen(serverSocket,6);
  printf("Listening...\n\n");

  int addrSize = sizeof(clientAddr);
  int newSocket = accept(serverSocket,(struct sockaddr*)&clientAddr,&addrSize);
  pthread_create(&tid,NULL,handler,&newSocket);

  while(1){
    scanf("%s",buffer);
    send(newSocket,buffer,strlen(buffer)+1,0);
  }

  close(serverSocket);
}