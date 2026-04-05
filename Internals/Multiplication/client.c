#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<string.h>
#include<pthread.h>

void print(int m,int n,int arr[][n]){
  for(int i=0;i<m;i++){
    for(int j=0;j<n;j++){
      printf("%d ",arr[i][j]);
    }
    printf("\n");
  }
}

void *handler(void *args){
  int *sockfd = args;
  int m,n,p,q;
  char buffer[1024];

  while(1){
    recv(*sockfd,buffer,sizeof(buffer),0);

    if(strcmp(buffer,"exit") == 0){
      
      break;
    }else{
      printf("Server: %s\n",buffer);
    }
  }
  
}

void main(){

}