#include "pthread_shop.h"

/* Global variables for all arrays to operate with in threads */
shop            shops[SHOPS_COUNT];
client          clients[CLIENTS_COUNT];
deliver         delivers[DELIVERS_COUNT];
pthread_mutex_t mutexes[SHOPS_COUNT];

int main() {
    shop_initialization(shops, clients, delivers);
    mutexes_initialization();
    
    /* To understanding in the client thread who they are */
    int client_selfidentifiacation[CLIENTS_COUNT];
    
    /* To understanding in the deliver thread who they are */
    int deliver_selfidentifiacation[DELIVERS_COUNT];
    
    int i;
    /* Define the thread arrays and create all threads */
    pthread_t client_threads[CLIENTS_COUNT];
    for(i = 0; i < CLIENTS_COUNT; i++) {
        client_selfidentifiacation[i] = i;
        if(0 != pthread_create(&client_threads[i], NULL, thread_client,\
                               &client_selfidentifiacation[i]))
        {
            perror("main: pthread_create fail\n");
            exit(EXIT_FAILURE);
        }
    }
    
    pthread_t delivers_threads[DELIVERS_COUNT];
    for(i = 0; i < DELIVERS_COUNT; i++) {
        deliver_selfidentifiacation[i] = i;
        if(0 != pthread_create(&delivers_threads[i], NULL, thread_deliver,\
                               &deliver_selfidentifiacation[i]))
        {
            perror("main: pthread_create fail\n");
            exit(EXIT_FAILURE);
        }
    }
    
    /* Join with all client threads */
    void* retval;
    for(i = 0; i < CLIENTS_COUNT; i++) {
        if(0 != pthread_join(client_threads[i], &retval)) {
            perror("main: pthread_join falied\n");
            exit(EXIT_FAILURE);
        }
    }
    /* Now all clients are get all what they need */
    /* But delivers don't know about it! */
    /* Need to let know delivers about it */
    for(i = 0; i < DELIVERS_COUNT; i++) {
        if(0 != pthread_cancel(delivers_threads[i])) {
            perror("main: pthread_cancel falied\n");
            exit(EXIT_FAILURE);
        }
    }
    /* All the deliver threads were signaled */
    /* Join with all deliver threads now */
    for(i = 0; i < DELIVERS_COUNT; i++) {
        if(0 != pthread_join(delivers_threads[i], &retval)) {
            perror("main: pthread_join falied\n");
            exit(EXIT_FAILURE);
        }
    }
    
    mutexes_finalization();
    
    return 0;
}
