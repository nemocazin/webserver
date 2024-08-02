/** INCLUDES */

#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <poll.h>

/** PROTOTYPES */

/** Initialisation du serveur */
int     initialisationServeur   (char *service,int connexions);

/** Boucle d'écoute des connexions sur le serveur */
int     boucleServeur           (int ecoute,int (*traitement)(int));

/** Convertion d'un nom en une adresse */
int     nomVersAdresse          (char *hote,struct sockaddr_storage *padresse);

/** Connexion sur le serveur et création du socket */
int     connexionServeur        (char *hote,int port);
