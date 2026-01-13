#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

void display(int m,int n,int *arr){
  for(int i=0;i<m;i++){
    for(int j=0;j<n;j++){
      printf("%d ",arr[i * n + j]);
    }
    printf("\n");
  }
}

void multiply(int m,int n,int p,int q,int *A,int *B,int C[][q]){
  if(n != p){
    printf("\nMatrix multiplication not possible\n");
    exit(-1);
  }

  for(int i=0;i<m;i++){
    for(int j=0;j<q;j++){
      C[i][j] = 0;
      for(int k=0;k<n;k++){
        C[i][j] += A[i*n + k] * B[k*q + j];
      }
    }
  }
}

void main(){
  struct sockaddr_in serverAddr;
  int buffer[2048];

  int sockfd = socket(AF_INET,SOCK_DGRAM,0);
  if(sockfd == -1){
    printf("\nError creating socket\n");
    exit(-1);
  }
  printf("Server socket created successfully\n");

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(6666);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  if(bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr)) == -1){
    printf("\nError binding socket\n");
    exit(-1);
  }
  printf("Bind to port number %d\n",ntohs(serverAddr.sin_port));

  int addrSize = sizeof(serverAddr);
  recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&serverAddr,&addrSize);

  int m = ntohl(buffer[0]);
  int n = ntohl(buffer[1]);
  int p = ntohl(buffer[2]);
  int q = ntohl(buffer[3]);

  int *A = &buffer[4];
  int *B = &buffer[4 + m*n];
  int C[m][q];

  printf("\nFirst Matrix\n");
  display(m,n,A);
  printf("\nSecond Matrix\n");
  display(p,q,B);
  multiply(m,n,p,q,A,B,C);

  int reply[2 + m*q];
  reply[0] = htonl(m);
  reply[1] = htonl(q);
  memcpy(&reply[2], C, sizeof(C));
  sendto(sockfd,reply,sizeof(reply),0,(struct sockaddr*)&serverAddr,addrSize);

  close(sockfd);
}