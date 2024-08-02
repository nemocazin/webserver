/** INCLUDES */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>
#include <pcap.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>

/** CONSTANTES */

#define MAX_STATS 5
#define KEY_NUM 5764

/** STRUCTURES */

typedef struct {
    char * ip;
    int compteur;
} Stats;

/** PROTOTYPES */

/** Print chaque paquet reçu */
void    print_paquets           (const unsigned char *paquet);

/** Print les statistiques des 5 IP sources les plus actives */
void    print_stats             (void);

/** Fait des stats sur les 5 IP sources les plus actives */
void    stat_paquet             (const unsigned char *paquet);

/** Analyse des arguments dans la commande */
int     analyseArguments        (int argc, char *argv[]);

/* Ecriture des stats dans la mémoire IPC */
void    stat_writing_IPC        (void);

/** Capture les paquets reçus */
void    catch_paquets           (unsigned char *user, const struct pcap_pkthdr *pkthdr, const unsigned char *paquet);

/** Ouvre l'interface spécifiée */
void    open_interface          (char *interface_reseau);
