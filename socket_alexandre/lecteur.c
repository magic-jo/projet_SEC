/********************************************************************************************/
/*                        APPLICATION : Processus Lecteur                                   */
/********************************************************************************************/

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

int main(void) {
    int sd; // socket descriptor
    struct sockaddr_un adrloc; // adresse de la socket locale
    int nbRecus; // nombre d'octets lus
    int adrsize; // taille en octets d'une adresse
    char reception[MAX_CHAR+1]; // buffer de réception

    // Étape 1 : Création de la socket
    sd = socket(AF_UNIX, SOCK_DGRAM, 0);
    CHECK_ERROR(sd = socket(AF_UNIX, SOCK_DGRAM, 0), -1, "La création de la socket de dialogue a échoué !");

    // Étape 2 : Remplir l'adresse de la socket
    adrloc.sun_family = AF_UNIX;
    strcpy(adrloc.sun_path, "Socket1");

    // Étape 3 : Liaison de la socket avec son adresse
    CHECK_ERROR(bind(sd, (const struct sockaddr *) &adrloc, sizeof(adrloc.sun_family) + strlen(adrloc.sun_path)), -1, "Échec du bind de la socket !");

    // Étape 4 : Réception des données
    nbRecus = recvfrom(sd, reception, MAX_CHAR+1, 0, NULL, NULL);
    if (nbRecus > 0) printf("LECTEUR > %s\n", reception);
    else printf("KO\n");

    // Pause
    getchar();

    // Close
    close(sd);
    unlink(adrloc.sun_path);

    return EXIT_SUCCESS;
}