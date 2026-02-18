#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

void reverse(char buffer[]){
  int n = strlen(buffer);
  for(int i=0;i<n/2;i++){
    char temp = buffer[i];
    buffer[i] = buffer[n-i-1];
    buffer[n-i-1] = temp;
  }
}

void main(){
  struct sockaddr_in serverAddr,clientAddr;
  char buffer[1024];
  
  int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  printf("Server socket created successfully\n");

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(4444);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  bind(serverSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
  printf("Bind to port number %d\n",ntohs(serverAddr.sin_port));

  listen(serverSocket,6);
  printf("Listening...\n");

  int addrSize = sizeof(clientAddr);
  int newSocket = accept(serverSocket,(struct sockaddr*)&clientAddr,&addrSize);

  recv(newSocket,buffer,sizeof(buffer),0);
  printf("\nData recieved: %s\n",buffer);
  reverse(buffer);
  send(newSocket,buffer,strlen(buffer)+1,0);
  
  close(newSocket);
  close(serverSocket);
}