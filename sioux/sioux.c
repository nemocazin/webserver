#include "sioux.h"
#include "serveur.h"
#include "analyste_http.h"
#include "thread.h"
#include "ipc.h"

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

void *gestionClient(void* sock){
    char buffer[MAX_BUFFER];
    char cmd[MAX_BUFFER];
    char page[MAX_BUFFER];
    char proto[MAX_BUFFER];
    char path[SIZE_PATH];
    char type[MAX_BUFFER];
    
    //Cast du paramètre
    int s = *((int*)sock);
    
    /* Obtient une structure de fichier */
    FILE *dialogue=fdopen(s,"a+");
    if(dialogue==NULL){ perror("gestionClient.fdopen"); exit(EXIT_FAILURE); }
    
    /* Echo */
    char ligne[MAX_LIGNE];
    while(fgets(ligne,MAX_LIGNE,dialogue)!=NULL){
    /* Analyse des commandes */
        if(sscanf(ligne, "%s %s %s", cmd, page, proto)!=3){
            printf("Erreur 501, mauvaise commande");
            exit(-1);
        }
        while(fgets(buffer,MAX_BUFFER,dialogue)!=NULL){
            if(strcmp(buffer,"\r\n")==0) break;
        }
        /* Gestion GET */
        if(strcasecmp(cmd,"GET")==0){
            int code=CODE_OK;
            struct stat fstat;
            sprintf(path,"%s%s",WEB_DIR,page);
            
            //Analyse de la réponse
            if(strstr(path, "reponse") != NULL){
                http_get_reponse_value(path);
                //Reconstitution du path
                char * strToken = strtok(path, "?");
                strcpy(path, strToken);
            }
            
            if(stat(path,&fstat)!=0 || !S_ISREG(fstat.st_mode)){
                sprintf(path,"%s/%s",WEB_DIR,PAGE_NOTFOUND);
                stat(path,&fstat);
                code=CODE_NOTFOUND;
            }
            
            strcpy(type,"text/html");
            char *end=page+strlen(page);
            //if(strcmp(end-4,".png")==0) strcpy(type,"image/png");
            if(strcmp(end-4,".jpg")==0) strcpy(type,"pictures/jpg");
            //if(strcmp(end-4,".gif")==0) strcpy(type,"image/gif");
            fprintf(dialogue,"HTTP/1.0 %d\r\n",code);
            fprintf(dialogue,"Server: CWeb\r\n");
            fprintf(dialogue,"Content-type: %s\r\n",type);
            fprintf(dialogue,"Content-length: %ld\r\n",fstat.st_size);
            fprintf(dialogue,"\r\n");
            fflush(dialogue);
            int fd=open(path,O_RDONLY);
            if(fd>=0){
                int bytes;
                while((bytes=read(fd,buffer,MAX_BUFFER))>0){
                    fwrite(buffer, bytes, 1, dialogue);
                }
                close(fd);
            }
        }
    }
    /* Termine la connexion */
    fclose(dialogue);
    return NULL;
}

int main(int argc,char *argv[]){
    int s, port;
    char port_str[30];
    key_t key = KEY_NUM;
    int shm_id;

    /** Lecture des arguments de la commande */
    port = analyseArguments(argc,argv);
    snprintf(port_str, sizeof(port_str), "%d", port);
    
    /** Connection au serveur */
    s=initialisationServeur(port_str, CONNEXIONS_MAXIMUM);
    if(s<0){ fprintf(stderr,"Erreur de connexion au serveur\n"); exit(EXIT_FAILURE); }    
    else {printf("Serveur initialisé !\n");}

    /** Connexion à la mémoire partagée */
    shm_id = createMemory(key);
    shm = attachMemory(shm_id);

    /** Boucle de gestion des clients */
    boucleServeur(s,(void*)creationThread);
    
    /** Suppression de la mémoire partagée */
    detachMemory(shm);
    deleteMemory(shm_id);

    /** On termine la connexion */
    shutdown(s,SHUT_RDWR);
    return 0;
}
