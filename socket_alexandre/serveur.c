/********************************************************************************************/
/*                        APPLICATION : Processus Client                                    */
/********************************************************************************************/

/* Utilisation*/
// ./client <pseudo-file-client>

/* Fichiers d'entête */
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

/* Macro-fonction */
#define CHECK_ERROR(val1, val2, msg)    \
    if ((val1) == (val2)) {             \
        perror(msg);                    \
        exit(EXIT_FAILURE);             \
    }

/* Constantes symboliques */
#define MAX_CHAR 80

/********************************************************************************************/

int main(int argc, char * argv[]) {
    int sd; // socket descriptor
    int pid = (int) getpid();
    struct sockaddr_un adrcli, ardserv;  // adresse de la socket client et du serveur
    struct sockaddr_un adrserv; // adresse de la socket serveur

    int nbEmis; // nombre d'octets envoyés
    int nbRecus; // nombre d'octets reçus
    int adrsize; // taille en octets d'une adresse

    char emission[MAX_CHAR+1] = "Ceci est un exemple de transmission de données."; // chaîne à transmettre
    char reception[MAX_CHAR+1]; // buffer de récéption

    // Étape 1 : Création de la socket
    sd = socket(AF_UNIX, SOCK_DGRAM, 0);
    CHECK_ERROR(sd = socket(AF_UNIX, SOCK_DGRAM, 0), -1, "La création de la socket de dialogue a échoué !");

    // Étape 2 : Remplir l'adresse de la socket du client
    adrcli.sun_family = AF_UNIX;
    if (argc == 2) {
        strcpy(adrloc.sun_path, argv[1]);
    }
    if (argc > 1) strcpy(adrcli.sun_path, argv[1]);
    else          strcpy(adrcli.sun_path, "SocketCli");

    // Étape 3 : Liaison de la socket du client avec son adresse
    CHECK_ERROR(bind(sd, (const struct sockaddr *) &adrserv, sizeof(adrserv.sun_family) + strlen(adrserv.sun_path)), -1, "Échec du bind de la socket du client !");

    // Étape 4 : Dialogue

    do {
        // Réception de data
        adrsize = sizeof(struct sockaddr_un);
        nbRecus = recvfrom(sd, reception, MAX_CHAR+1, 0, NULL, NULL);
    }


    printf("Client-%d>", pid);
    fgets(emission, MAX_CHAR, stdin);
    emission[strlen(emission)-1] = '\0'; // pour supprimer le LF (line-feed) mis dans le buffer

    do {

    nbEmis = sendto(sd, emission, strlen(emission), 0, (const struct sockaddr *) &adrserv, sizeof(adrserv.sun_family) + strlen(adrserv.sun_path));
    CHECK_ERROR(nbEmis, 0, "Aucune donnée envoyée...");

    // Mise du client en réception de data
    nbRecus = recvfrom(sd, reception, MAX_CHAR+1, 0, NULL, NULL);
    if (nbRecus > 0) printf("LECTEUR > %s\n", reception);
    else printf("KO\n");


    } while (!strcmp(emission, "fin") && !strcmp(reception, "fin"));

    // Pause
    getchar();

    // Unlink
    unlink(adrcli.sun_path);
    
    // Close
    close(sd);

    return EXIT_SUCCESS;
}