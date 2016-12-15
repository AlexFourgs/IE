#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFLEN 512
#define PORT 9930

void diep(char *s){
  perror(s);
  exit(1);
}

int main(void){

  int received_packet = 0 ;
  int echeance, packet_envoye, task_period, task_deadline ;
  echeance = packet_envoye = task_period = task_deadline = 0 ;
  int j = 0 ;
  struct sockaddr_in si_me, si_other;
  int s, i, slen=sizeof(si_other);
  char buf[BUFLEN];
  char* p;
  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1){
    diep("socket");
  }

  memset((char *) &si_me, 0, sizeof(si_me));
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(PORT);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(s, &si_me, sizeof(si_me))==-1)
    diep("bind");

  while (strcmp(buf,"9999") != 0) {
    if (recvfrom(s, buf, BUFLEN, 0, &si_other, &slen)==-1){
	     diep("recvfrom()");
    }
    received_packet++ ;
    printf("Data: %s\n\n", buf);

    p=strtok(buf,",");
    printf("avant = %s\n", p);
    if(strcmp(p,"2")==0){
      while (p!=NULL){
        if(j==1){
          printf("%s\n", p);
          packet_envoye = atoi(p) ;
        }
        else if(j == 2){
          printf("%s\n", p);

          task_period = atoi(p) ;
        }
        else if(j == 3){
          printf("%s\n", p);

          task_deadline = atoi(p) ;
        }
        j++;
        p = strtok(NULL, ",");
      }
    }
    else if(strcmp(p,"1")==0){
      while (p!=NULL){
        echeance = atoi(p) ;
        p = strtok(NULL, ",");
      }
    }
  }

  printf("Il y a eu %d echeances\n", echeance); // On enlève le paquet 9999
  printf("Le gumstix envoie %d packet\n", packet_envoye); // On enlève le paquet 9999
  printf("task_period = %d\n", task_period); // On enlève le paquet 9999
  printf("task_deadline = %d\n", task_deadline); // On enlève le paquet 9999

  printf("J'ai reçu %d packet\n", received_packet-3); // On enlève le paquet 9999

  close(s);
  return 0;
}
