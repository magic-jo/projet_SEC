#include <stdio.h>
#include <stdlib.h>

int main() {
    FILE *fp;
    char buffer[1024];

    // Exécuter la commande Python et ouvrir un flux de lecture
    fp = popen("python3 python.py", "r");

    if (fp == NULL) {
        fprintf(stderr, "Erreur lors de l'exécution de la commande.\n");
        return 1;  // Quitter le programme avec une erreur
    }

    // Lire et afficher le résultat de l'exécution
    while (fgets(buffer, sizeof(buffer)-1, fp) != NULL) {
        printf("%s", buffer);
    }

    // Fermer le flux
    pclose(fp);

    return 0;  // Terminer le programme avec succès
}
