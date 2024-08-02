#include "serveur.h"

int initialisationServeur(char *service,int connexions){
    struct addrinfo precisions,*resultat,*origine;
    int statut;
    int s;

    /* Construction de la structure adresse */
    memset(&precisions,0,sizeof precisions);
    precisions.ai_family=AF_UNSPEC;
    precisions.ai_socktype=SOCK_STREAM;
    precisions.ai_flags=AI_PASSIVE;
    statut=getaddrinfo(NULL,service,&precisions,&origine);
    if(statut<0){ perror("initialisationServeur.getaddrinfo"); exit(EXIT_FAILURE); }
    struct addrinfo *p;
    for(p=origine,resultat=origine;p!=NULL;p=p->ai_next)
    if(p->ai_family==AF_INET6){ resultat=p; break; }

    /* Creation d'une socket */
    s=socket(resultat->ai_family,resultat->ai_socktype,resultat->ai_protocol);
    if(s<0){ perror("initialisationServeur.socket"); exit(EXIT_FAILURE); }

    /* Options utiles */
    int vrai=1;
    if(setsockopt(s,SOL_SOCKET,SO_REUSEADDR,&vrai,sizeof(vrai))<0){
    perror("initialisationServeur.setsockopt (REUSEADDR)");
    exit(EXIT_FAILURE);
    }
    if(setsockopt(s,IPPROTO_TCP,TCP_NODELAY,&vrai,sizeof(vrai))<0){
    perror("initialisationServeur.setsockopt (NODELAY)");
    exit(EXIT_FAILURE);
    }

    /* Specification de l'adresse de la socket */
    statut=bind(s,resultat->ai_addr,resultat->ai_addrlen);
    if(statut<0){ freeaddrinfo(origine); shutdown(s,SHUT_RDWR); return -1; }

    /* Liberation de la structure d'informations */
    freeaddrinfo(origine);

    /* Taille de la queue d'attente */
    statut=listen(s,connexions);
    if(statut<0){ shutdown(s,SHUT_RDWR); return -2; }

    return s;
}

int boucleServeur(int ecoute,int (*traitement)(int)){
    int dialogue;
    while(1){

        /* Attente d'une connexion */
        if((dialogue=accept(ecoute,NULL,NULL))<0) return -1;

        /* Passage de la socket de dialogue a la fonction de traitement */
        if(traitement(dialogue)<0){ shutdown(ecoute,SHUT_RDWR); return 0;}
        }
    }

int nomVersAdresse(char *hote,struct sockaddr_storage *padresse){
    struct addrinfo *resultat,*origine;
    int statut;
    statut=getaddrinfo(hote,NULL,NULL,&origine);
    if(statut==EAI_NONAME) return -1;
    if(statut<0){ perror("nomVersAdresse.getaddrinfo"); exit(EXIT_FAILURE); }
    struct addrinfo *p;
    for(p=origine,resultat=origine;p!=NULL;p=p->ai_next)
    if(p->ai_family==AF_INET6){ resultat=p; break; }
    memcpy(padresse,resultat->ai_addr,resultat->ai_addrlen);
    return 0;
}
    
int connexionServeur(char *hote,int port){
    int s;
    struct sockaddr_in adresse;
    socklen_t taille=sizeof adresse;
    //int statut;

    /* Creation d'une socket */
    s=socket(PF_INET,SOCK_STREAM,0);
    if(s<0){
        perror("connexionServeur.socket");
        exit(-1);
    }

    /* Connection de la socket a l'hote */
    adresse.sin_family=AF_INET;
    if(nomVersAdresse(hote,(void *)&adresse)<0) return -1;
    adresse.sin_port=htons(port);
    if(connect(s,(struct sockaddr *)&adresse,taille)<0) return -1;
    else return s;
}

