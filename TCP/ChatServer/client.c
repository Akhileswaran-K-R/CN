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
  if(clientSocket == -1){
    printf("\nError creating socket\n");
    exit(-1);
  }
  printf("Client socket created successfully\n");

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(4444);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if(connect(clientSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) == -1){
    printf("\nError establishing connection\n");
    exit(-1);
  }
  printf("Connected to server\n");

  printf("\nEnter your message(Type exit to disconnect)\n\n");
  while(1){
    printf("Msg: ");
    fgets(buffer, sizeof(buffer), stdin);
    send(clientSocket,buffer,strlen(buffer)+1,0);

    if(strcmp(buffer,"exit\n") == 0){
      printf("\nDisconnected from server\n");
      close(clientSocket);
      exit(0);
    }

    recv(clientSocket,buffer,sizeof(buffer),0);
    printf("Status: %s\n\n",buffer);
  }
}