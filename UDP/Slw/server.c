#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

void main(){
  struct sockaddr_in serverAddr,clientAddr;
  int packet;

  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  printf("Server socket created successfully\n");

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(6666);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  bind(sockfd,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
  printf("Bind to port number %d\n\n",ntohs(serverAddr.sin_port));

  int addrSize = sizeof(clientAddr);
  srand(time(0));
  float loss = 0.3;

  while(1){
    recvfrom(sockfd,&packet,sizeof(packet),0,(struct sockaddr*)&clientAddr,&addrSize);

    if(packet == -1){
      printf("\n");
    }else{
      if((float)rand() / RAND_MAX < loss){
        printf("Packet %d dropped.\n",packet);
        continue;
      }
      printf("Received packet %d -> Sending ACK %d\n",packet,packet);
      sendto(sockfd,&packet,sizeof(packet),0,(struct sockaddr*)&clientAddr,addrSize);
    }
  }
}