#include "analyste_http.h"

int http_get_reponse_value(const char* url) {
    char* char_value = strstr(url, "reponse");
    if (char_value) {
        char_value += 8; // Effaçage "reponse="
        fill_csv(atoi(char_value));
        }
    return 0;
}

void fill_csv(int pilote) {
    FILE *fichier = fopen("www/vote.csv", "r+"); // Ouvrir le fichier en mode lecture et écriture
    if (fichier == NULL) {
        printf("Erreur lors de l'ouverture du fichier 'vote.csv'.\n");
        return;
    }

    // Chercher la ligne correspondant au candidat
    char ligne[100];
    while (fgets(ligne, sizeof(ligne), fichier)) {
        int piloteFichier;
        sscanf(ligne, "Pilote %d,", &piloteFichier);

        if (piloteFichier == pilote) {
            // Incrémenter le nombre de votes pour ce candidat
            int nombreDeVotes = 0;
            sscanf(ligne, "Pilote %d,%d", &piloteFichier, &nombreDeVotes);
            nombreDeVotes++;
            fseek(fichier, -1 * (int)strlen(ligne), SEEK_CUR); // Revenir au début de la ligne
            fprintf(fichier, "Pilote %d,%d\n", pilote, nombreDeVotes);
            break;
        }
    }

    // Fermer le fichier
    fclose(fichier);
}



