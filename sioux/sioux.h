/** INCLUDES */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

/** DEFINES */

#define CONNEXIONS_MAXIMUM   10
#define MAX_LIGNE		     1024
#define WEB_DIR              "./www"
#define PAGE_NOTFOUND        "error.html"
#define MAX_BUFFER           1024
#define CODE_OK              200
#define CODE_NOTFOUND        404
#define SIZE_PATH            2048
#define THREAD_MAX           10
#define KEY_NUM              5764

/** PROTOTYPES */

/** Analyse des arguments dans la commande */
int     analyseArguments        (int argc, char *argv[]);

/** Gestion des clients */
void*   gestionClient           (void* sock);
