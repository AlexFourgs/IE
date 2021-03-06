#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/**

    PROTOCOLE DES MESSAGES
    1 : Échéances manquées
    2 : Données de connexion (paquets envoyés, vitesse...)
    3 : Données accélération, vitesse et déplacement.
    4 : Heure début enregistrement données
    5 : Heure fin enregistrement données

**/


#define BUFLEN 512
#define PORT 9930

void diep(char *s){
  perror(s);
  exit(1);
}

/**
    Fonction qui écrit les données de la connexion (nombre de paquet envoyé, période, etc...), et les écrit dans un fichier .csv
**/
void add_data_csv(int task_period, int task_deadline, int paquet_send, int paquet_receive, int deadline_fail){
    FILE* csv_file = NULL;

    csv_file = fopen("connection.csv", "a+");

    if (csv_file != NULL){
        fprintf(csv_file, "%d,%d,%d,%d,%d\n", task_period, task_deadline, paquet_send, paquet_receive, deadline_fail);
        fclose(csv_file);
    }
}


int main(void){

  FILE *fichier=NULL;
  struct timeval start,checkpoint;
  int accX=0,vitesseX=0,positionX=0;
  /*int acc[2]={0,0};
  int vel[2]={0,0};
  int pos[2]={0,0};*/

  // Variable pour la connexion
  int received_packet = 0 ;
  int echeance, packet_envoye, task_period, task_deadline ;
  echeance = packet_envoye = task_period = task_deadline = 0 ;

  int j = 0 ;

  // Variable serveur UDP
  struct sockaddr_in si_me, si_other;
  int s, i, slen=sizeof(si_other);
  char buf[BUFLEN];
  char* p;


  if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1){
    diep("socket");
  }

  fichier = fopen("save","w+");

  if(fichier==NULL){
    printf("Error can't open or create file  \n");
    exit(1);
  }


  memset((char *) &si_me, 0, sizeof(si_me));
  si_me.sin_family = AF_INET;
  si_me.sin_port = htons(PORT);
  si_me.sin_addr.s_addr = htonl(INADDR_ANY);

  if (bind(s, &si_me, sizeof(si_me))==-1)
    diep("bind");

 //gettimeofday(&start, 0);
 //printf("start: %d \n",start.tv_sec, start.tv_usec);
  while (strcmp(buf,"9999") != 0) {
    if (recvfrom(s, buf, BUFLEN, 0, &si_other, &slen)==-1){
	     diep("recvfrom()");
    }
    received_packet++ ;
    //printf("Data: %s\n\n", buf);

    p=strtok(buf,",");

    // Réception configuration connexion (period, deadline, nb paquet)
    if(strcmp(p,"2")==0){
      j=0;
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
    // Réception nb échéance(s) manquée(s)
    else if(strcmp(p,"1")==0){
      while (p!=NULL){
        echeance = atoi(p) ;
        p = strtok(NULL, ",");
      }
    }
    // Réception donnée accX, vitesseX, positionX
    else if (strcmp(p,"3")==0){
        j=0;
        while (p!=NULL){
          if(j==1){
            printf("Accélération x = %s\n", p);
            accX = atoi(p) ;
          }
          else if(j == 2){
            printf("Vitesse x = %s\n", p);
            vitesseX = atof(p) ;
          }
          else if(j == 3){
            printf("Position x = %s\n", p);
            positionX = atof(p) ;
          }
          j++;
          p = strtok(NULL, ",");
       }
    }
    else if (strcmp(p,"4")==0){
        j=0;
        while (p!=NULL){
          if(j==1){
            //printf("%s\n", p);
            //time_s = atoll(p) ;
          }
          else if(j == 2){
            //printf("%s\n", p);
            //time_us = atoll(p) ;
          }
          j++;
          p = strtok(NULL, ",");
      }
    }
      // Négatif
      /*if (accX>127){
        accX=accX-255;
      }
      gettimeofday(&checkpoint, 0);

      	if(accX>=-4 && accX<=4){
			accX=0;
		}

		if (tempAcc=1){
			acc[1]=accX;
			tempAcc=0;
		}
		else if(tempAcc=0){
			acc[0]=accX;
			tempAcc=1;
		}

		vel[1]=vel[0]+acc[0]+((acc[1]-acc[0])>>1);
		pos[1]=pos[0]+vel[0]+((vel[1]-vel[0])>>1);

		vel[0]=vel[1];
		pos[0]=pos[1];
		printf("Values : %d %d %d %d %d %d\n",((checkpoint.tv_sec-start.tv_sec)*1000)+((checkpoint.tv_usec-start.tv_usec)/1000),accX,vel[0],vel[1],pos[0],pos[1]);

		fprintf(fichier,"%d %d %d %d %d %d\n",((checkpoint.tv_sec-start.tv_sec)*1000)+((checkpoint.tv_usec-start.tv_usec)/1000),accX,vel[0],vel[1],pos[0],pos[1]);
        */
      //printf("Accelero : %d %d %d\n",checkpoint.tv_sec, checkpoint.tv_usec,accX);

    }

  //printf("time_s = %lld, time_us = %lld\n", time_s, time_us);
  printf("Il y a eu %d echeance(s) manquée(s)\n", echeance); // On enlève le paquet 9999
  printf("Le gumstix envoie %d packet\n", packet_envoye); // On enlève le paquet 9999
  printf("task_period = %d\n", task_period); // On enlève le paquet 9999
  printf("task_deadline = %d\n", task_deadline); // On enlève le paquet 9999

  printf("J'ai reçu %d packet\n", received_packet-4); // On enlève le paquet 9999
  add_data_csv(task_period, task_deadline, packet_envoye, received_packet-3, echeance);

  fclose(fichier);

  close(s);
  return 0;
}
