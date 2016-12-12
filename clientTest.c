#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#define BUFLEN 512
#define PORT 9930


#define TASK_PERIOD 10000 /* période de la tâche en microsecondes*/ // Plus TASK_PERIOD est haut, plus l'envoi de paquet sera lent. (1000000 = 1s)
#define TASK_DEADLINE 1000 /* depassement autorisé*/

void diep(char *s)
{
  perror(s);
  exit(1);
}


  #define SRV_IP "192.168.10.2"
  /* diep(), #includes and #defines like in the server */

 int main(void)
  {

    struct timeval start,checkpoint;
    long long diff;

      struct sockaddr_in si_other;
      int s, slen=sizeof(si_other);
      char buf[BUFLEN];
      int val1,val2,val3;
      int tmp = 0;

      val1=val2=val3=4;

           if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1)
             diep("socket");

          memset((char *) &si_other, 0, sizeof(si_other));
        si_other.sin_family = AF_INET;
        si_other.sin_port = htons(PORT);
        if (inet_aton(SRV_IP, &si_other.sin_addr)==0) {
           fprintf(stderr, "inet_aton() failed\n");
           exit(1);
          }

	/* on récupère le temps avant de rentrer dans la boucle*/
          gettimeofday(&start, 0);

          while (tmp < 100) {


	    /*mesure du temps écoulé depuis le dernier passage ici- attention section critique possible- */
           gettimeofday(&checkpoint, 0);
	   diff=(checkpoint.tv_sec-start.tv_sec) * 1000000L + (checkpoint.tv_usec-start.tv_usec);
	   if (diff < TASK_PERIOD ) ; /* On cadence l'execution toutes les TASK_PERIOD*/
	   else {
	     gettimeofday(&start, 0); /* On réinitialise le compteur */
	     printf("temps écoulé=%lld usec\n",diff);
	     if (diff > TASK_PERIOD + TASK_DEADLINE) printf ("***echeance manquée \n"); /* si la condition temps réelle n'est pas respectée */
	     else {  /*si la condition temps réel est respectée*/

	       /*envoi des informations*/
	       sprintf(buf, "%d %d %d\n", val1,val2,val3);
	       if (sendto(s, buf, BUFLEN, 0, &si_other, slen)==-1)
		 diep("sendto()");

	       /* changement des informations (simul*/
	       tmp++;
	       if (tmp ==254) tmp=0;
	       val1=val2=val3=tmp;
	     }

	   }
	  }
    int val4 = 9999 ;
    sprintf(buf, "%d", val4);
    if (sendto(s, buf, BUFLEN, 0, &si_other, slen)==-1){
      diep("sendto()");
    }
          close(s);
	  return 0;
  }
