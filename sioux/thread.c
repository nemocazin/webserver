#include "thread.h"

void creationThread(int s){
    int *sock_ptr = (int *)malloc(sizeof(int));
    *sock_ptr = s;
    pthread_t thread;
    pthread_create(&thread, NULL, gestionClient, (void*)sock_ptr);
    pthread_join(thread, NULL);
}
