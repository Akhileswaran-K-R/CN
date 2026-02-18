#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>

void readMatrix(int m,int n,int arr[][n]){
  for(int i=0;i<m;i++){
    for(int j=0;j<n;j++){
      scanf("%d",&arr[i][j]);
    }
  }
}

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

void displayMatrix(int m,int n,int arr[][n]){
  printf("\nResultant Matrix\n");
  for(int i=0;i<m;i++){
    for(int j=0;j<n;j++){
      printf("%d ",arr[i][j]);
    }
    printf("\n");
  }
}

void main(){
  struct sockaddr_in serverAddr;
  int buffer[1024];

  int sockfd = socket(AF_INET,SOCK_DGRAM,0);
  printf("Client socket created successfully\n");

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(4444);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  int m,n,p,q;
  printf("\nEnter the size of first matrix\n");
  scanf("%d %d",&m,&n);
  int A[m][n];
  printf("Enter the elements of first matrix\n");
  readMatrix(m,n,A);
  
  printf("\nEnter the size of second matrix\n");
  scanf("%d %d",&p,&q);
  int B[p][q];
  printf("Enter the elements of second matrix\n");
  readMatrix(p,q,B);

  if(n != p){
    printf("\nMatrix multiplication cannot be performed\n");
    exit(-1);
  }
  
  int package[4 + m*n + p*q];
  package[0] = htonl(m);
  package[1] = htonl(n);
  package[2] = htonl(p);
  package[3] = htonl(q);
  
  int idx = 4;
  copyMatrix(m,n,&idx,A,package);
  copyMatrix(p,q,&idx,B,package);
  sendto(sockfd,package,sizeof(package),0,(struct sockaddr*)&serverAddr,sizeof(serverAddr));

  int addr_size = sizeof(serverAddr);
  recvfrom(sockfd,buffer,sizeof(buffer),0,(struct sockaddr*)&serverAddr,&addr_size);
  
  int row = ntohl(buffer[0]);
  int col = ntohl(buffer[1]);
  int C[row][col];

  idx = 2;
  originalMatrix(row,col,&idx,C,buffer);
  displayMatrix(row,col,C);

  close(sockfd);
}