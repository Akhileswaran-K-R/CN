#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<pthread.h>

void *handler(void *args){
	char buffer[1024];
  int *clientSocket = args;
	while(1){
		recv(*clientSocket,buffer,sizeof(buffer),0);
		printf("%s",buffer);
	}
	return NULL;
}

void main(){
	struct sockaddr_in serverAddr;
	char buffer[1024];
	pthread_t recv_thread;
	
	int clientSocket = socket(AF_INET,SOCK_STREAM,0);
  printf("Client socket created successfully\n");
	
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(8080);
	serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	connect(clientSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr));

  printf("Enter your name: ");
  scanf("%s",buffer);
  getchar();
  send(clientSocket,buffer,strlen(buffer)+1,0);
  printf("Connected to server\n\n");
	
	pthread_create(&recv_thread,NULL,handler,&clientSocket);
	while(1){
		fgets(buffer,sizeof(buffer),stdin);
		send(clientSocket,buffer,strlen(buffer)+1,0);

    if(strcmp(buffer,"exit\n") == 0){
      printf("\nDisconnected from server\n");
      close(clientSocket);
      exit(0);
    }
	}
}