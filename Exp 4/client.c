#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

void acceptMatrix(int m,int n,int arr[][n]){
  printf("Enter the matrix elements\n");
  for(int i=0;i<m;i++){
    for(int j=0;j<n;j++){
      scanf("%d",&arr[i][j]);
    }
  }
}

void display(int m,int n,int *arr){
  printf("\nResult matrix\n");
  for(int i=0;i<m;i++){
    for(int j=0;j<n;j++){
      printf("%d ",arr[i * n + j]);
    }
    printf("\n");
  }
}

void main(){
  struct sockaddr_in serverAddr;
  int buffer[1024];

  int sockfd = socket(AF_INET,SOCK_DGRAM,0);
  if(sockfd == -1){
    printf("\nError creating socket\n");
    exit(-1);
  }
  printf("Client socket created successfully\n");

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(6666);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  int m,n,p,q;
  printf("Enter the size of first matrix\n");
  scanf("%d %d",&m,&n);
  int A[m][n];
  acceptMatrix(m,n,A);

  printf("\nEnter the size of second matrix\n");
  scanf("%d %d",&p,&q);
  int B[p][q];
  acceptMatrix(p,q,B);

  int packet[4 + m*n + p*q];

  packet[0] = htonl(m);
  packet[1] = htonl(n);
  packet[2] = htonl(p);
  packet[3] = htonl(q);

  memcpy(&packet[4], A, sizeof(A));
  memcpy(&packet[4 + m*n], B, sizeof(B));
  sendto(sockfd,packet,sizeof(packet),0,(struct sockaddr*)&serverAddr,sizeof(serverAddr));

  int addrSize = sizeof(serverAddr);
  recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&serverAddr,&addrSize);
  
  int row = ntohl(buffer[0]);
  int col = ntohl(buffer[1]);
  int *C = &buffer[2];
  display(row,col,C);
  
  close(sockfd);
}