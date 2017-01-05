#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <errno.h>
#include <string.h>
#include "I2C/i2c-dev.h"
#include "I2C/i2cbusses.h"
#include "I2C/util.h"

#define BUFLEN 512
#define PORT 9930


void diep(char *s)
{
  perror(s);
  exit(1);
}


#define SRV_IP "192.168.10.2"
/* diep(), #includes and #defines like in the server */



static void help(void) __attribute__ ((noreturn));

static void help(void){
  	fprintf(stderr, "Usage: i2cget [-f] [-y] I2CBUS CHIP-ADDRESS [DATA-ADDRESS [MODE]]\n"
  		"  I2CBUS is an integer or an I2C bus name\n"
  		"  ADDRESS is an integer (0x03 - 0x77)\n"
  		"  MODE is one of:\n"
  		"    b (read byte data, default)\n"
  		"    w (read word data)\n"
  		"    c (write byte/read byte)\n"
  		"    Append p for SMBus PEC\n");
  	exit(1);
}

static int check_funcs(int file, int size, int daddress, int pec){
  	unsigned long funcs;

  	/* check adapter functionality */
  	if (ioctl(file, I2C_FUNCS, &funcs) < 0) {
  		fprintf(stderr, "Error: Could not get the adapter "
  			"functionality matrix: %s\n", strerror(errno));
  		return -1;
  	}

  	switch (size) {
      case I2C_SMBUS_BYTE:
  		  if (!(funcs & I2C_FUNC_SMBUS_READ_BYTE)) {
  		      fprintf(stderr, MISSING_FUNC_FMT, "SMBus receive byte");
  			    return -1;
        }
  		  if (daddress >= 0 && !(funcs & I2C_FUNC_SMBUS_WRITE_BYTE)) {
          fprintf(stderr, MISSING_FUNC_FMT, "SMBus send byte");
  			  return -1;
  		  }
  		break;

  	  case I2C_SMBUS_BYTE_DATA:
  		  if (!(funcs & I2C_FUNC_SMBUS_READ_BYTE_DATA)) {
          fprintf(stderr, MISSING_FUNC_FMT, "SMBus read byte");
  			  return -1;
  		  }
  		break;

  	  case I2C_SMBUS_WORD_DATA:
  		  if (!(funcs & I2C_FUNC_SMBUS_READ_WORD_DATA)) {
          fprintf(stderr, MISSING_FUNC_FMT, "SMBus read word");
  			  return -1;
  		  }
  		break;
  	}

  if (pec && !(funcs & (I2C_FUNC_SMBUS_PEC | I2C_FUNC_I2C))) {
    fprintf(stderr, "Warning: Adapter does "
  	"not seem to support PEC\n");
  }

  return 0;
}

  static int confirm(const char *filename, int address, int size, int daddress,
  		   int pec)
  {
  	int dont = 0;

  	fprintf(stderr, "WARNING! This program can confuse your I2C "
  		"bus, cause data loss and worse!\n");

  	/* Don't let the user break his/her EEPROMs */
  	if (address >= 0x50 && address <= 0x57 && pec) {
  		fprintf(stderr, "STOP! EEPROMs are I2C devices, not "
  			"SMBus devices. Using PEC\non I2C devices may "
  			"result in unexpected results, such as\n"
  			"trashing the contents of EEPROMs. We can't "
  			"let you do that, sorry.\n");
  		return 0;
  	}

  	if (size == I2C_SMBUS_BYTE && daddress >= 0 && pec) {
  		fprintf(stderr, "WARNING! All I2C chips and some SMBus chips "
  			"will interpret a write\nbyte command with PEC as a"
  			"write byte data command, effectively writing a\n"
  			"value into a register!\n");
  		dont++;
  	}

  	fprintf(stderr, "I will read from device file %s, chip "
  		"address 0x%02x, ", filename, address);
  	if (daddress < 0)
  		fprintf(stderr, "current data\naddress");
  	else
  		fprintf(stderr, "data address\n0x%02x", daddress);
  	fprintf(stderr, ", using %s.\n",
  		size == I2C_SMBUS_BYTE ? (daddress < 0 ?
  		"read byte" : "write byte/read byte") :
  		size == I2C_SMBUS_BYTE_DATA ? "read byte data" :
  		"read word data");
  	if (pec)
  		fprintf(stderr, "PEC checking enabled.\n");

  	fprintf(stderr, "Continue? [%s] ", dont ? "y/N" : "Y/n");
  	fflush(stderr);
  	if (!user_ack(!dont)) {
  		fprintf(stderr, "Aborting on user request.\n");
  		return 0;
  	}

  	return 1;
  }

  int i2cget_ntm(int argc, char *argv[])
  {
  	char *end;
  	int res, i2cbus, address, size, file;
  	int daddress;
  	char filename[20];
  	int pec = 0;
  	int flags = 0;
  	int force = 0, yes = 0;

  	/* handle (optional) flags first */
  	while (1+flags < argc && argv[1+flags][0] == '-') {
  		switch (argv[1+flags][1]) {
  		case 'f': force = 1; break;
  		case 'y': yes = 1; break;
  		default:
  			fprintf(stderr, "Error: Unsupported option "
  				"\"%s\"!\n", argv[1+flags]);
  			help();
  			exit(1);
  		}
  		flags++;
  	}

  	if (argc < flags + 3)
  		help();

  	i2cbus = lookup_i2c_bus(argv[flags+1]);
  	if (i2cbus < 0)
  		help();

  	address = parse_i2c_address(argv[flags+2]);
  	if (address < 0)
  		help();

  	if (argc > flags + 3) {
  		size = I2C_SMBUS_BYTE_DATA;
  		daddress = strtol(argv[flags+3], &end, 0);
  		if (*end || daddress < 0 || daddress > 0xff) {
  			fprintf(stderr, "Error: Data address invalid!\n");
  			help();
  		}
  	} else {
  		size = I2C_SMBUS_BYTE;
  		daddress = -1;
  	}

  	if (argc > flags + 4) {
  		switch (argv[flags+4][0]) {
  		case 'b': size = I2C_SMBUS_BYTE_DATA; break;
  		case 'w': size = I2C_SMBUS_WORD_DATA; break;
  		case 'c': size = I2C_SMBUS_BYTE; break;
  		default:
  			fprintf(stderr, "Error: Invalid mode!\n");
  			help();
  		}
  		pec = argv[flags+4][1] == 'p';
  	}

  	file = open_i2c_dev(i2cbus, filename, sizeof(filename), 0);
  	if (file < 0
  	 || check_funcs(file, size, daddress, pec)
  	 || set_slave_addr(file, address, force))
  		exit(1);

  	if (!yes && !confirm(filename, address, size, daddress, pec))
  		exit(0);

  	if (pec && ioctl(file, I2C_PEC, 1) < 0) {
  		fprintf(stderr, "Error: Could not set PEC: %s\n", strerror(errno));
  		close(file);
  		exit(1);
  	}

  	switch (size) {
  	case I2C_SMBUS_BYTE:
  		if (daddress >= 0) {
  			res = i2c_smbus_write_byte(file, daddress);
  			if (res < 0)
  				fprintf(stderr, "Warning - write failed\n");
  		}
  		res = i2c_smbus_read_byte(file);
  		break;
  	case I2C_SMBUS_WORD_DATA:
  		res = i2c_smbus_read_word_data(file, daddress);
  		break;
  	default: /* I2C_SMBUS_BYTE_DATA */
  		res = i2c_smbus_read_byte_data(file, daddress);
  	}
  	close(file);

  	if (res < 0) {
  		fprintf(stderr, "Error: Read failed\n");
  		exit(2);
  	}

  	//printf("0x%0*x\n", size == I2C_SMBUS_WORD_DATA ? 4 : 2, res);

  	return res;
  }



int main(int argc, char *argv[]){
    // Attributs pour le client UDP
    int task_period = 100000 ; /* période de la tâche en microsecondes*/ // Plus task_period est haut, plus l'envoi de paquet sera lent. (1000000 = 1s)
    int task_deadline = 10000 ; /* depassement autorisé*/
    int packet_to_send = 100 ;
    int stop_connection = 1 ;

    // Attributs pour envoyer les données au serveur
    int echeance_manque = 0 ;


    struct timeval start,checkpoint;
    long long diff;
    struct sockaddr_in si_other;
    int s, slen=sizeof(si_other);
    char buf[BUFLEN];
    int tmp = 0;

    if(argc == 5){ // Si il y a des arguments
        task_period = atoi(argv[1]);
        task_deadline = atoi(argv[2]);
        packet_to_send = atoi(argv[3]);
        stop_connection = atoi(argv[4]);
    }

    char* paramX[6] = {"i2cget_ntm", "-f", "-y", "3", "0x1d", "0x29"};
    //char* paramY[6] = {"i2cget_ntm", "-f", "-y", "3", "0x1d", "0x2B"};
    //char* paramZ[6] = {"i2cget_ntm", "-f", "-y", "3", "0x1d", "0x2D"};


    if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))==-1){
        diep("socket");
    }

    memset((char *) &si_other, 0, sizeof(si_other));
    si_other.sin_family = AF_INET;
    si_other.sin_port = htons(PORT);

    if (inet_aton(SRV_IP, &si_other.sin_addr)==0) {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

	/* on récupère le temps avant de rentrer dans la boucle*/
    gettimeofday(&start, 0);

    while (tmp < packet_to_send) {

	    /*mesure du temps écoulé depuis le dernier passage ici- attention section critique possible- */
        gettimeofday(&checkpoint, 0);
        diff=(checkpoint.tv_sec-start.tv_sec) * 1000000L + (checkpoint.tv_usec-start.tv_usec);
        if (diff < task_period ) ; /* On cadence l'execution toutes les task_period*/
	    else {
            gettimeofday(&start, 0); /* On réinitialise le compteur */
            printf("temps écoulé=%lld usec\n",diff);

            if (diff > task_period + task_deadline){
                echeance_manque++ ;
                printf ("***echeance manquée \n"); /* si la condition temps réelle n'est pas respectée */
            }
	        else {  /*si la condition temps réel est respectée*/

                /*envoi des informations*/
                sprintf(buf, "3,%d\n",i2cget_ntm(6, paramX));
                printf("3,%d\n",i2cget_ntm(6, paramX));
                //printf ("%d %d %d \n",i2cget_ntm(6, paramX),i2cget_ntm(6, paramY),i2cget_ntm(6, paramZ));

                if (sendto(s, buf, BUFLEN, 0, &si_other, slen)==-1){
                    diep("sendto()");
                }
                /* changement des informations (simul*/
                tmp++;
            }
        }
    }

    /*
        On envoie les échéances manquées
    */
    sprintf(buf, "1,%d", echeance_manque);
    if (sendto(s, buf, BUFLEN, 0, &si_other, slen)==-1){
        diep("sendto()");
    }

    /*
        On envoie les informations sur le client au serveur.
        (nombre de paquets envoyés, période, etc...)
    */
    sprintf(buf, "2,%d,%d,%d", packet_to_send, task_period, task_deadline);
    if (sendto(s, buf, BUFLEN, 0, &si_other, slen)==-1){
        diep("sendto()");
    }

    /*
        Si stop_connection est différent de 0,
        on envoie le signal de fin de transmission
        pour que le serveur s'arrête.
    */
    if(stop_connection != 0){
        int stop = 9999 ;
        sprintf(buf, "%d", stop);
        if (sendto(s, buf, BUFLEN, 0, &si_other, slen)==-1){
          diep("sendto()");
        }
    }

    close(s);

	return 0;
}
