#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int exit = 0;

void multiply(int m,int n,int p,int q,int a[][n],int b[][q],int c[][q]){
  for(int i=0;i<m;i++){
    for(int j=0;j<q;j++){
      c[i][j] = 0;
      for(int k=0;k<n;k++){
        c[i][j] += a[i][k] * b[k][j];
      }
    }
  }
}

void *handler(void *args){
  int *sockfd = args;
  int m,n,p,q;
  char buffer[1024];

  while(1){
    recv(*sockfd,buffer,sizeof(buffer),0);

    if(strcmp(buffer,"Matrix") == 0){
      recv(*sockfd,&m,sizeof(int),0);
      recv(*sockfd,&n,sizeof(int),0);
      recv(*sockfd,&p,sizeof(int),0);
      recv(*sockfd,&q,sizeof(int),0);

      int a[m][n],b[p][q],c[m][q];
      recv(*sockfd,a,sizeof(int)*m*n,0);
      recv(*sockfd,b,sizeof(int)*m*n,0);
      multiply(m,n,p,q,a,b,c);
      send(*sockfd,c,sizeof(int)*m*q,0);

      printf("Product send to client\n");
    }else if(strcmp(buffer,"exit") == 0){
      pthread_mutex_lock(&mutex);
      if(exit_count == 0)
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