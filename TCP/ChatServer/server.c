#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<pthread.h>
#include<arpa/inet.h>
#include<string.h>
#define MAX 20

typedef struct {
  int sockfd;
  char name[MAX];
}client_t;

client_t clients[MAX];
int n=0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

void addClient(int sockfd,char name[]){
	pthread_mutex_lock(&mutex);
	clients[n].sockfd = sockfd;
  strcpy(clients[n++].name,name);
  printf("%s joined the server\n",name);
	pthread_mutex_unlock(&mutex);
}

void removeClient(int sockfd){
	pthread_mutex_lock(&mutex);
	for(int i=0;i<n;i++){
		if(clients[i].sockfd == sockfd){
      printf("%s left the server\n",clients[i].name);
			clients[i] = clients[--n];
			break;
		}
	}
	pthread_mutex_unlock(&mutex);
}

void broadcast(int sockfd,char name[],char buffer[]){
  pthread_mutex_lock(&mutex);
  char msg[1024];
  sprintf(msg,"%s: %s",name,buffer);
  for(int i=0;i<n;i++) {
    if(clients[i].sockfd != sockfd){
      send(clients[i].sockfd,msg,strlen(msg)+1,0);
    }
  }
  pthread_mutex_unlock(&mutex);
}

void *handler(void *args){
	char buffer[1024],name[1024];
	int sockfd = *((int*)args);
  free(args);

  recv(sockfd,name,sizeof(name),0);
  addClient(sockfd,name);
	
	while(1){
		recv(sockfd,buffer,sizeof(buffer),0);
    if(strcmp(buffer,"exit\n") == 0){
      break;
    }
	  broadcast(sockfd,name,buffer);
	}
	close(sockfd);
	removeClient(sockfd);
	pthread_detach(pthread_self());
	return NULL;
}

void main(){
	struct sockaddr_in serverAddr, clientAddr;
	pthread_t tid;
	
	int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
  printf("Server socket created successfully\n");

	serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(8080);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    
  bind(serverSocket,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
  printf("Bind to port number %d\n",ntohs(serverAddr.sin_port));

  listen(serverSocket,6);
  printf("Listening...\n\n");
    
	while(1){
		int addrSize = sizeof(clientAddr);
		int newSocket = accept(serverSocket,(struct sockaddr*)&clientAddr,&addrSize);

    int *pclient = malloc(sizeof(int));
    *pclient = newSocket;
		pthread_create(&tid,NULL,handler,pclient);
	}
}