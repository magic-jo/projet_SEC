/****************************************************/
/* Application : clienTCP                            */
/* Date : 13/01/2023                                */
/* Version : 2                                      */
/* Compilation : gcc clientUdp.c -o clientUdpc          */
/* Usage : ./clientUdpc [adrIPserv] [portServ]   [adrIPcli] */
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
#define REMOTEIP "127.0.0.1"
#define REMOTEPORT 3000

#define CHECKERROR(var,val,msg)     if (var==val) {perror(msg); exit(1);}

int main(int argc, char * argv[])
{
    
int sd1; //descripteur de socket de dialogue
struct sockaddr_in addrServ, addrCli;
char buff[MAXCAR+1];
int erreur, nbcar;
int adrlg=sizeof(struct sockaddr_in);

//Etape 1 - Creation de la socket

sd1=socket(AF_INET, SOCK_STREAM, 0);

CHECKERROR(sd1,-1, "Creation fail !!!\n");

//Etape2 - Adressage du destinataire

addrServ.sin_family=AF_INET;

if (argc>=3)
{
    addrServ.sin_port=htons(atoi(argv[2]));
    addrServ.sin_addr.s_addr=inet_addr(argv[1]);
}
else
{
addrServ.sin_port=htons(REMOTEPORT);
addrServ.sin_addr.s_addr=inet_addr(REMOTEIP);
}

//Etape 3 - demande d'ouverture de connexion

CHECKERROR(connect(sd1, (const struct sockaddr *)&addrServ, sizeof(struct sockaddr_in)),-1, "Connexion fail !!!\n");

do
{
printf("CLIENT '%d'>", getpid());
fgets(buff, MAXCAR,stdin);
buff[strlen(buff)-1]='\0';

//gets(buff);
nbcar=send(sd1,buff, strlen(buff)+1, 0);
//printf("CLIENT '%d' : ", getpid());
//nbcar=recvfrom(sd1,buff, MAXCAR+1, 0, &addrServ, &adrlg); 

//if (nbcar) printf(" '%s' \n", buff);
}

while (strcmp(buff, "FIN") && strcmp(buff, "fin"));
    
//Etape6 - on ferme la socket ce qui va liberer le port
close(sd1);

return EXIT_SUCCESS;
}


    
    
