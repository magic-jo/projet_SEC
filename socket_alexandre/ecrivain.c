/********************************************************************************************/
/*                        APPLICATION : Processus Écrivain                                  */
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
    struct sockaddr_un adrlect; // adresse de la socket locale
    int nbEmis; // nombre d'octets envoyés
    int adrsize; // taille en octets d'une adresse
    char emission[MAX_CHAR+1] = "Ceci est un exemple de transmission de données."; // chaîne à transmettre

    // Étape 1 : Création de la socket
    sd = socket(AF_UNIX, SOCK_DGRAM, 0);
    CHECK_ERROR(sd = socket(AF_UNIX, SOCK_DGRAM, 0), -1, "La création de la socket de dialogue a échoué !");

    // Étape 2 : Remplir l'adresse de la socket du destinataire
    adrlect.sun_family = AF_UNIX;
    strcpy(adrlect.sun_path, "Socket1");

    // Étape 3 : Envoi de data
    nbEmis = sendto(sd, emission, strlen(emission), 0, (const struct sockaddr *) &adrlect, sizeof(adrlect.sun_family) + strlen(adrlect.sun_path));
    CHECK_ERROR(nbEmis, 0, "Aucune donnée envoyée...");

    // Pause
    getchar();

    // Close
    close(sd);

    return EXIT_SUCCESS;
}