#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/time.h>

void main(){
  struct sockaddr_in serverAddr;
  struct timeval tv;

  int sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  printf("Client socket created successfully\n");

  tv.tv_sec = 1;
  tv.tv_usec = 0;
  setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));

  serverAddr.sin_family = AF_INET;
  serverAddr.sin_port = htons(6666);
  serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

  int c,total,window;

  while(1){
    printf("\n\nEnter a choice\n");
    printf("1.Stop & Wait\n");
    printf("2.Go-Back-N\n");
    printf("3.Selective Repeat\n");
    printf("4.Exit\n");
    scanf("%d", &c);

    if(c == 4){
      close(sockfd);
      exit(0);
    }

    printf("\nTotal packets: ");
    scanf("%d", &total);

    if(c == 1){
      window = 1;
    }else{
      printf("Window size: ");
      scanf("%d",&window);
    }

    int base = 0,next = 0;
    int ack = -1;
    int acked[1000] = {0};

    sendto(sockfd,&ack,sizeof(ack),0,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
    while(base < total){
      while(next < base + window && next < total){
        if(c == 3 && acked[next]){
          next++;
          continue;
        }
        printf("\nSending packet %d",next);
        sendto(sockfd,&next,sizeof(next),0,(struct sockaddr*)&serverAddr,sizeof(serverAddr));
        next++;
      }

      int addrSize = sizeof(serverAddr);
      int n = recvfrom(sockfd,&ack,sizeof(ack),0,(struct sockaddr*)&serverAddr,&addrSize);

      if(n >= 0){
        printf("\nACK received for packet: %d",ack);

        if(c == 1 || c == 2){
          if(ack == base){
            base++;
          }
        }else if(c == 3){
          acked[ack] = 1;
          while(base < total && acked[base]){
            base++;
          }
        }
      }else{
        printf("\n\nTimeout at packet %d! Resending...",base);
        next = base;
      }
    }
  }
}