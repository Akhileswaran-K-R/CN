#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

void copyMatrix(int m,int n,int *idx,int arr[][n],int package[]){
  for(int i=0;i<m;i++){
    for(int j=0;j<n;j++){
      package[(*idx)++] = htonl(arr[i][j]);
    }
  }
}

void originalMatrix(int m,int n,int *idx,int arr[][n],int buffer[]){
  for(int i=0;i<m;i++){
    for(int j=0;j<n;j++){
      arr[i][j] = ntohl(buffer[(*idx)++]);
    }
  }
}

void multiply(int m,int n,int p,int q,int A[][n],int B[][q],int C[][q]){
  for(int i=0;i<m;i++){
    for(int j=0;j<q;j++){
      C[i][j] = 0;
      for(int k=0;k<p;k++){
        C[i][j] += A[i][k] * B[k][j];
      }
    }
  }
}

void displayMatrix(int m,int n,int arr[][n]){
  for(int i=0;i<m;i++){
    for(int j=0;j<n;j++){
      printf("%d ",arr[i][j]);
    }
    printf("\n");
  }
}

void main(){
  struct sockaddr_in serverAddr,clientAddr;
  int buffer[2048];
  
  int sockfd = socket(AF_INET,SOCK_DGRAM,0);
  if(sockfd == -1){
    printf("\nError creating socket\n");
    exit(-1);
  }
  printf("Server socket created successfully\n");
  
  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(4444);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");
  
  if(bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) == -1){
    printf("\nError binding socket\n");
    exit(-1);
  }
  printf("Bind to port number %d\n",ntohs(serverAddr.sin_port));
  
  int addr_size = sizeof(clientAddr);
  recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&clientAddr,&addr_size);
  
  int m = ntohl(buffer[0]);
  int n = ntohl(buffer[1]);
  int p = ntohl(buffer[2]);
  int q = ntohl(buffer[3]);
  
  int A[m][n],B[p][q],C[m][q],idx=4;
  originalMatrix(m,n,&idx,A,buffer);
  originalMatrix(p,q,&idx,B,buffer);
  
  printf("\nFirst Matrix\n"); 
  displayMatrix(m,n,A);
  printf("\nSecond Matrix\n");
  displayMatrix(p,q,B);
  multiply(m,n,p,q,A,B,C);
  
  int package[2 + m*q];
  package[0] = htonl(m);
  package[1] = htonl(q);

  idx = 2;
  copyMatrix(m,q,&idx,C,package);
  sendto(sockfd,package,sizeof(package),0,(struct sockaddr*)&clientAddr,sizeof(clientAddr));
  
  close(sockfd);
}