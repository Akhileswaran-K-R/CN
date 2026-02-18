#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

void main(){
  struct sockaddr_in serverAddr;
  char buffer[1024];
  
  int clientSocket = socket(AF_INET,SOCK_STREAM,0);
  printf("Client socket created successfully\n");

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(4444);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  connect(clientSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
  printf("Connected to server\n");

  strcpy(buffer,"Hello from client");
  send(clientSocket,buffer,strlen(buffer)+1,0);
  recv(clientSocket,buffer,sizeof(buffer),0);
  printf("Data recieved: %s\n",buffer);
  
  close(clientSocket);
}