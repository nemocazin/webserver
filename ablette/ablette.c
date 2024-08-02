#include "ablette.h"
#include "ipc.h"

Stats stats[MAX_STATS];
int port;
char *shm;

int analyseArguments(int argc, char *argv[]){
    int option_index =0;
    int opt_value;
    int num_port;
    static struct option long_options[] = {
                   {"port",     required_argument, 0,  'p' },
                   {0,         0,                 0,  0 }
                   };
    while((opt_value = getopt_long(argc, argv, "p:0", long_options, &option_index)) != -1){
        switch(opt_value){
            case'?' :
                printf("Mauvaise syntaxe, utilisez : -p 'port' ou  -port 'port'\n");
                break;
                
            case 0 :
                
            case 'p':
                if (sscanf(optarg, "%d", &num_port)== 1){
                    printf("Bon port : %d\n", num_port);
                }
                else{
                    printf("Mauvaise syntaxe, utilisez : -p 'port' ou  -port 'port'\n");
                }
                break;
        }
    }
    return num_port;
}

void print_paquets(const unsigned char *paquet){
    struct ip *ip_entete = (struct ip*)(paquet + 14); /** On estime que les entête Ethernet font 14 octets */

    /** Vérification si le paquet est un IPv4 */
    if (ip_entete->ip_v == 4) {
        /** Vérification sur l'IP de destination est l'adresse de loopback*/
        if (ip_entete->ip_dst.s_addr == htonl(INADDR_LOOPBACK)) {
            printf("Paquet reçu sur l'interface de loopback :\n");
            printf("IP source : %s\n", inet_ntoa(ip_entete->ip_src));
            printf("IP destination: %s\n", inet_ntoa(ip_entete->ip_dst));
            printf("\n");
        }
    }
}

void print_stats(void){
    printf("Top 5 IP sources :\n");
    for (int i = 0; i < MAX_STATS; i++){
        if (stats[i].compteur > 0){
            printf("IP Source : %s - Nombre de paquets reçus : %d\n", stats[i].ip, stats[i].compteur);
        }
    }
}

void stat_paquet(const unsigned char *paquet){
    struct ip *ip_entete = (struct ip*)(paquet + 14); /** On estime que les entête Ethernet font 14 octets */

    if (ip_entete->ip_p == IPPROTO_TCP){
        struct tcphdr *tcp_entete = (struct tcphdr*)(paquet + 14 + (ip_entete->ip_hl << 2));

        /** Vérification si le paquet va sur le port */
        if (ntohs(tcp_entete->th_dport) == port){
            int i;

            /** Vérifie si l'IP source est dans la structure */
            for (i = 0; i < MAX_STATS; i++){
                if (stats[i].ip ==  inet_ntoa(ip_entete->ip_src)){
                    stats[i].compteur++;
                    break;
                }
            }

            /** Si l'IP source n'est pas dans la structure, l'ajoute */
            if (i == MAX_STATS){
                for (i = 0; i < MAX_STATS; i++){
                    if (stats[i].compteur == 0){
                        stats[i].ip =  inet_ntoa(ip_entete->ip_src);
                        stats[i].compteur = 1;
                        break;
                    }
                }
            }
        }
    }
}

void stat_writing_IPC(){
    char str_compteur[100] = "";
    for(int i = 0; i < MAX_STATS; i++){
        if(stats[i].ip != NULL){
            writeMemory(shm, stats[i].ip);
            sprintf(str_compteur, "%d", stats[i].compteur);
            writeMemory(shm, str_compteur);
        }
    }
}

void catch_paquets(unsigned char *user, const struct pcap_pkthdr *pkthdr, const unsigned char *paquet){
    //print_paquets(paquet);
    stat_paquet(paquet);
    stat_writing_IPC();
    //print_stats();
}

void open_interface(char *interface_reseau){
    char buf_error[PCAP_ERRBUF_SIZE];
    char *filter_exp = "";
    struct bpf_program fp;
    pcap_t *interface;
    
    /** Ouvrir l'interface */
    interface = pcap_open_live(interface_reseau, BUFSIZ, 1, 1000, buf_error);
    if(interface == NULL){
        fprintf(stderr, "Impossible d'ouvrir l'interface %s ! : %s\n", interface_reseau, buf_error);
        exit(EXIT_FAILURE);
    }
    
    /** Compilation du filtre */
    if(pcap_compile(interface, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1){
        fprintf(stderr, "Erreur compilation pour le filtre !\n");
        exit(EXIT_FAILURE);
    }
    
    /** Application du filtre */
    if(pcap_setfilter(interface, &fp) == -1){
        fprintf(stderr, "Erreur application du filtre !\n");
        exit(EXIT_FAILURE);
    }
    
    /** Capture des paquets */
    pcap_loop(interface, 0, catch_paquets, NULL);

    /** Fermeture de l'interface */
    pcap_close(interface);
}

int main(int argc,char *argv[]){
    char *interface_reseau = "eth0";
    key_t key = KEY_NUM;
    int shm_id;

    /** Lecture des arguments de la commande */
    port = analyseArguments(argc,argv);
    if (port == 0) { port = 80; } // Port par défaut 

    /** Création de la mémoire partagée */
    shm_id = createMemory(key);
    shm = attachMemory(shm_id);

    /** Ouverture de l'interface*/
    open_interface(interface_reseau);

    /** Suppression de la mémoire partagée */
    detachMemory(shm);
    // Mémoire supprimée dans sioux

    return 0;
}
