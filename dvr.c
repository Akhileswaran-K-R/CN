#include<stdio.h>
#define MAX 20
#define INF 999

typedef struct{
  int dist[MAX];
  int next[MAX];
}router;

void main(){
  int n;
  printf("Enter the number of routers: ");
  scanf("%d", &n);
  router r[n];

  printf("\nEnter the adjacency matrix (0 for no connection):\n");
  for(int i=0;i<n;i++){
    for(int j=0;j<n;j++){
      scanf("%d", &r[i].dist[j]);

      if(i == j){
        r[i].dist[j] = 0;
      }else if(r[i].dist[j] == 0){
        r[i].dist[j] = INF;
      }
      r[i].next[j] = j;
    }
  }

  int updated;
  do{
    updated = 0;

    for(int i=0;i<n;i++){
      for(int j=0;j<n;j++){
        for(int k=0;k<n;k++){
          if(r[i].dist[j] > r[i].dist[k] + r[k].dist[j]){
            r[i].dist[j] = r[i].dist[k] + r[k].dist[j];
            r[i].next[j] = r[i].next[k];
            updated = 1;
          }
        }
      }
    }
  }while(updated);

  printf("\nFinal Routing Table:\n");
  for(int i=0;i<n;i++){
    printf("\nRouter %c\n", (char)(i + 65));
    printf("Dest\tCost\tNext Hop\n");

    for(int j=0;j<n;j++){
      if(r[i].dist[j] == INF){
        printf("%c\t-\t-\n", (char)(j + 65));
      }else{
        printf("%c\t%d\t%c\n",(char)(j + 65),r[i].dist[j],(char)(r[i].next[j] + 65));
      }
    }
  }
}
