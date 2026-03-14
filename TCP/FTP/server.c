#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

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
  printf("Listening...\n\n");

  int addrSize = sizeof(clientAddr);
  while(1){
    int newSocket = accept(serverSocket,(struct sockaddr*)&clientAddr,&addrSize);
    pid_t pid = fork();

    if(pid < 0){
      printf("\nFork error\n");
    }else if(pid == 0){
      close(serverSocket);
      recv(newSocket,buffer,sizeof(buffer),0);
      FILE *fp = fopen(buffer,"r");

      if(fp){
        sprintf(buffer,"Server PID: %d\nFile Content:\n",getpid());
        send(newSocket,buffer,strlen(buffer)+1,0);

        while(fgets(buffer,sizeof(buffer),fp) != NULL){
          send(newSocket,buffer,strlen(buffer)+1,0);
        }

        fclose(fp);
      }else{
        sprintf(buffer,"Server PID: %d\nFile Not Found",getpid());
        send(newSocket,buffer,strlen(buffer)+1,0);
      }

      close(newSocket);
      exit(0);
    }else{
      printf("Client Connected\n");
      close(newSocket);
    }
  }

  close(serverSocket);
}