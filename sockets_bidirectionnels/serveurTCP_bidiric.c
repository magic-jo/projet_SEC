#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAXCAR  80
#define LOCALIP "127.0.0.1"
#define LOCALPORT 3000
#define READER_PORT 5000 // Serveur = lecteur, client = écrivain
#define WRITER_PORT 5001 // Serveur = écrivain, client = lecteur

#define NB_MAX_CONNECTIONS 6

#define CHECKERROR(var,val,msg)     if (var==val) {perror(msg); exit(EXIT_FAILURE);}

#define CHECKERROR_SOCK(var,val,msg, socket)     if (var==val) {perror(msg); close(socket); exit(EXIT_FAILURE);}

void write_in_file(char *message, char *nom_fichier) {
    FILE *fichier = fopen(nom_fichier, "a");
    if (fichier == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        exit(1);
    }
    fprintf(fichier, "%s\n", message);
    fclose(fichier);
}

int main() {
    int socketEcoute, socketDialogue[NB_MAX_CONNECTIONS];
    struct sockaddr_in serverAddr, clientAddr[NB_MAX_CONNECTIONS];
    socklen_t addrSize = sizeof(struct sockaddr_in);

    char buffer[1024];
    pid_t pidFils[NB_MAX_CONNECTIONS]; //Permet de recuperer le pid du processus fils cree

    // Créer une socket
    socketEcoute = socket(AF_INET, SOCK_STREAM, 0);
    if (socketEcoute == -1) {
        perror("Erreur lors de la création de la socket");
        exit(EXIT_FAILURE);
    }

    // Configuration de l'adresse du serveur
    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(LOCALPORT);

    // Liaison de la socket à l'adresse
    if (bind(socketEcoute, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) == -1) {
        perror("Erreur lors de la liaison de la socket");
        close(socketEcoute);
        exit(EXIT_FAILURE);
    }

    // Mettre la socket en mode écoute
    if (listen(socketEcoute, NB_MAX_CONNECTIONS) == -1) {
        perror("Erreur lors de la mise en écoute de la socket");
        close(socketEcoute);
        exit(EXIT_FAILURE);
    }

    printf("En attente de connexions...\n");

    // Création des processus et sockets client
    for(int i = 0; i < NB_MAX_CONNECTIONS; i++) {

        // Création des sockets
        socketDialogue[i] = accept(socketEcoute, (struct sockaddr*)&(clientAddr[i]), &addrSize);
        CHECKERROR_SOCK(socketDialogue[i],-1, "Erreur lors de l'acceptation de la connexion\n", socketEcoute); // On coupe toutes les connections si une seule échoue! Comportement à modifier?
        printf("Connexion acceptée depuis %s:%d\n", inet_ntoa(clientAddr[i].sin_addr), ntohs(clientAddr[i].sin_port));

        // Création des processus fils
        pidFils[i] = fork(); // Faut-il déclarer pidFils[] comme variable globale? à vérifier

        if (pidFils[i] == 0) {
            // On est dans le fils
            printf("Je suis le processus fils %ld, j'ai été créé en %ld-ième.\n", getpid(), i);

            // Différencier le comportement selon le port :
            // clientAddr[i].sinPort == 5000 ? -> client = écrivain, donc serveur = lecteur
            // Coder le comportement en tant que lecteur

            // clientAddr[i].sinPort == 5001 ? -> client = lecteur, donc serveur = écrivain
            // Coder le comportement en tant que écrivain
            // Envoyer des données au client
            strcpy(buffer, "ma super commande");

            // PB !! Comment savoir sur quelle socket écrire? Quel [i]??? En fonction de l'adresse IP?
            // send(socketDialogue[i], buffer, strlen(buffer), 0);

            exit(EXIT_SUCCESS);
        } else {
            // On est dans le processus père
            close(socketDialogue[i]); //fermeture des sockets de dialogue utilisées par le père (possédée par le fils désormais)
        }
    }
    
    // Reste du code exécuté par le père
    for (int i = 0; i < NB_MAX_CONNECTIONS; i++) wait(NULL); // Attends que chaque processus fils se termine





/* 
    // Communication bidirectionnelle avec le client
    while (1) {
        // Recevoir des données du client
        ssize_t bytesRead = recv(socketDialogue, buffer, sizeof(buffer), 0);
        if (bytesRead <= 0) {
            perror("Erreur lors de la réception des données");
            break;
        }

        // Afficher les données reçues
        buffer[bytesRead] = '\0';
        printf("Client dit : %s\n", buffer);

        // Envoyer des données au client
        printf("Entrez une réponse pour le client : ");
        fgets(buffer, sizeof(buffer), stdin);
        send(socketDialogue, buffer, strlen(buffer), 0);
    }
 */
    // Fermer les sockets
    close(socketDialogue);
    close(socketEcoute);

    return 0;
}
