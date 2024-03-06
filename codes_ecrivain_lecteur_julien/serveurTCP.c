/****************************************************/
/* Application : serveurTCP                            */
/* Date : 13/01/2023                                */
/* Version : 1                                      */
/* Compilation : gcc serveurTCP.c -o serveurTCP     */
/* Usage : ./serveurUdp  [port]                     */
/****************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>


#define MAXCAR  80
#define LOCALIP "127.0.0.1"
#define LOCALPORT 3000

#define CHECKERROR(var,val,msg)     if (var==val) {perror(msg); exit(1);}

void write_in_file(char *message, char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "a");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(1);
    }
    fprintf(fichier, "%s\n", message);
    fclose(fichier);
}

int main(char * argv[], int argc)
{
int se; // descripteur de la socket d'ecoute    
int sd; //descripteur de socket de dialogue
struct sockaddr_in addrserv, addrcli;
char buff[MAXCAR+1];
int erreur;
int adrlg; //recupere la longueur de l'adresse
int nbcar; // Indique le nb de car recu ou emis
int pidfils; //Permet de recuperer le pid du processus fils cree

char nom_fichier[] = "message_recu.txt"; // Nom du fichier dans lequel écrire

//Etape 1 - Creation de la socket

se=socket(AF_INET, SOCK_STREAM, 0);

CHECKERROR(se,-1, "Creation fail !!!\n");



//Etape2 - Adressage de la socket de la socket d'ecoute

addrserv.sin_family=AF_INET;
addrserv.sin_addr.s_addr=INADDR_ANY;//On recoit des data sur toutes les adresses IP de la machine

//Definition d'un port de dialogue du lecteur
if (argc==2)
    addrserv.sin_port=htons(atoi(argv[1]));
else
addrserv.sin_port=htons(LOCALPORT);

//Etape 3 - Affectation de l'adresse a la socket avec controle d'erreur
CHECKERROR(bind(se, (const struct sockaddr *)&addrserv,sizeof(addrserv)), -1, "Erreur de bind !!!\n");

//Etape 4 - definir le nombre de connexions simultannees

listen(se,8);


//Etape 5 - mise du serveur a l'ecoute des demandes de connexion

printf("\n SERVEUR TCP \n\n\n ");

while(1)
{

sd=accept(se, NULL, NULL); //le serveur se met a l'ecoute des demande de connexion
CHECKERROR(sd,-1, "L'ouverture de connexion n'a pas fonctionne !!!\n");

pidfils=fork();

if (pidfils) {  // dans le processus pere
	printf("Ouverture d'une nouvelle connexion geree par %d  !!! \n", pidfils);
	close (sd); //fermeture de la socket de dialogue utilisee par le fils
	}
	
else  // dans le processus fils
	while(1){
		pidfils=getpid();
		close(se); //fermeture de la socket d'ecoute du pere
		nbcar=recv(sd,buff, MAXCAR+1, 0);
		 
		if (nbcar){
			printf("\n SERVEUR %d : '%s' ",pidfils, buff);   
			write_in_file(buff, nom_fichier);
			}
		
		else printf("Ca ne fonctionne pas !!!\n");
		
		if (strcmp(buff, "FIN")==0 || strcmp(buff, "fin")==0) 
			{  
			printf("\n Arret du fils %d  !!! \n", pidfils);
			close(sd);
			exit(1);
			}
	}
}


return EXIT_SUCCESS;
}


    
    
