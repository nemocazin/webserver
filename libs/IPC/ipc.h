/** INCLUDES */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

/** CONSTANTES */

#define SHM_SIZE 1024  // Taille de la mémoire partagée

/** PROTOTYPES */

/** Création de la mémoire partagée */
int     createMemory    (key_t key);

/** Attachement de la mémoire partagée au processus */
char*   attachMemory    (int shm_id);

/** Détachement de la mémoire partagée au processus */
void    detachMemory    (char *shm);

/** Suppression de la mémoire partagée */
void    deleteMemory    (int shm_id);

/** Ecriture dans la mémoire partagée */
void    writeMemory     (char *shm, char *data);

/** Lecture de la mémoire partagée */
char *  readMemory      (char *shm);
