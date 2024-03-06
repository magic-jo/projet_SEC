#include <sys/socket.h>
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/un.h>
#include <stdlib.h>
#include <fcntl.h>

#define CHECK_ERROR(val1, val2, msg)     \
    if (val1 == val2) {                   \
        perror(msg);                      \
        exit(1);                          \
    }

#define MAXCHAR 200

int read_file(const char *nom_fichier, char *contenu) {
    FILE *fichier = fopen(nom_fichier, "r");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return -1;
    }

    fseek(fichier, 0, SEEK_END);
    long taille = ftell(fichier);
    rewind(fichier);

    if (taille >= MAXCHAR) {
        perror("La taille du fichier dépasse la limite");
        fclose(fichier);
        return -1;
    }

    fread(contenu, sizeof(char), taille, fichier);
    contenu[taille] = '\0'; // Ajout d'un caractère de fin de chaîne
    fclose(fichier);
    return 0;
}


int main() {
    int sd; // descripteur de socket
    struct sockaddr_un adrlect; // adresse de la socket du destinataire
    int nbemis; // indique le nombre d'octets émis
    char emission[MAXCHAR + 1]; // buffer pour la fonction recvfrom

    // Etape 1 : création de la socket
    sd = socket(AF_UNIX, SOCK_DGRAM, 0);
    CHECK_ERROR(sd, -1, "La création de la socket de dialogue a échoué\n");

    // Etape 2 : remplir l'adresse de la socket du destinataire
    adrlect.sun_family = AF_UNIX;
    strcpy(adrlect.sun_path, "socket1");

    // Lecture du contenu du fichier
    char nom_fichier[] = "commande.txt";
    if (read_file(nom_fichier, emission) == -1) {
        exit(1);
    }

    // Etape 3 : envoi de data
    nbemis = sendto(sd, emission, strlen(emission) + 1, 0, (struct sockaddr *)&adrlect, sizeof(adrlect));
    CHECK_ERROR(nbemis, -1, "Aucune donnée envoyée !\n");

    // Etape finale : fermeture de la socket
    close(sd);

    return 0;
}

