#include "ipc.h"

int createMemory(key_t key){
    int shm_id;
    if ((shm_id = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0){
        perror("Erreur lors de la création de la mémoire partagée");
        exit(1);
    }
    return shm_id;
}

char* attachMemory(int shm_id){
    char *shm = shmat(shm_id, NULL, 0);
    if (shm == (char *) -1){
        perror("Erreur lors de l'attachement de la mémoire partagée");
        exit(1);
    }
    return shm;
}

void detachMemory(char *shm){
    if (shmdt(shm) == -1){
        perror("Erreur lors du détachement de la mémoire partagée");
        exit(1);
    }
}

void deleteMemory(int shm_id){
    if (shmctl(shm_id, IPC_RMID, NULL) == -1){
        perror("Erreur lors de la suppression de la mémoire partagée");
        exit(1);
    }
}

void writeMemory(char *shm, char* data){
    sprintf(shm, "%s", (char*)data);
}

char * readMemory(char *shm){
    printf("Contenu de la mémoire partagée : %s\n", shm);
    return shm;
}
