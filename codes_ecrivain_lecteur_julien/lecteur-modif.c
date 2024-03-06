#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>

#define CHECK_ERROR(val1, val2, msg) \
    if(val1 == val2) { \
        perror(msg); \
        exit(1); \
    }

#define MAXCHAR 200

void write_in_file(char *message, char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "w");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(1);
    }
    fprintf(fichier, "%s", message);
    fclose(fichier);
}

int main() {
    int sd; //descripteur de socket
    struct sockaddr_un adrloc; //adresse de la socket locale
    int nbrecu; //indique le nombre d'octets lus
    int adrsize; //indique la taille en octets d'une adresse*
    char reception[MAXCHAR + 1]; //buffer pour la fonction recvfrom
    char nom_fichier[] = "message_recu.txt"; // Nom du fichier dans lequel écrire

    // Etape 1 : création de la socket
    sd = socket(AF_UNIX, SOCK_DGRAM, 0);
    CHECK_ERROR(sd, -1, "La création de la socket de dialogue a échoué\n");

    // Etape 2 : remplir l'adresse de la socket
    adrloc.sun_family = AF_UNIX;
    strcpy(adrloc.sun_path, "socket1");

    // Etape 3 : liaison de la socket avec son adresse
    CHECK_ERROR(bind(sd, (const struct sockaddr *)&adrloc, sizeof(adrloc.sun_family) + strlen(adrloc.sun_path)), -1, "Echec du bind() de la socket\n");

    // Etape 4 : réception de data
    nbrecu = recvfrom(sd, reception, MAXCHAR + 1, 0, NULL, NULL);

    if (nbrecu < 0) {
        printf("LECTEUR> %s\n", reception);
        // Écrire le message reçu dans un fichier
        write_in_file(reception, nom_fichier);
    } else {
        printf("KO\n");
    }

    // Etape finale : suppression du pseudo fichier et fermeture de la socket
    printf("La socket a été créée au niveau disque dur\nAppuyer sur une touche pour arrêter le programme et supprimer le pseudo fichier de la socket\n");
    // getchar(); // Faire une pause du programme

    unlink(adrloc.sun_path);

    close(sd);
}

