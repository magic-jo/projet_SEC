#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    // Boucle pour créer 6 processus fils
    for (int i = 0; i < 6; i++) {
        pid_t pid = fork(); // Crée un nouveau processus fils

        if (pid < 0) {
            // Erreur lors de la création du processus fils
            perror("Erreur lors de la création du processus fils");
            exit(EXIT_FAILURE);
        } else if (pid == 0) {
            // Code exécuté par le processus fils
            printf("Je suis le processus fils %d avec PID %d\n", i+1, getpid());
            exit(EXIT_SUCCESS);
        } else {
            // Code exécuté par le processus père
            printf("Processus père avec PID %d a créé le processus fils %d avec PID %d\n", getpid(), i+1, pid);
        }
    }

    // Code exécuté uniquement par le processus père après la création des fils
    for (int i = 0; i < 6; i++) {
        wait(NULL); // Attends que chaque processus fils se termine
    }

    return 0;
}
