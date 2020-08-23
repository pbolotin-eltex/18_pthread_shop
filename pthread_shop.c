#include "pthread_shop.h"

int shop_initialization(shop* shops, client* clients, deliver* delivers) {
    srandom((unsigned int)time(NULL));
    int i;
    for(i = 0; i < SHOPS_COUNT; i++) {
        shops[i].goodies = 450 + random()%101;
        printf("Shop %d was opened with %d goodies\n", i, shops[i].goodies);
    }
    printf("\n");
    
    for(i = 0; i < CLIENTS_COUNT; i++) {
        clients[i].how_many_goodies_are_wanted = 9800 + random()%401;
        printf("Client %d needs %d goodies\n", i, clients[i].how_many_goodies_are_wanted);
    }
    printf("\n");
    
    for(i = 0; i < DELIVERS_COUNT; i++) {
        delivers[i].how_many_goodies_were_added = 0;
        printf("Deliver %d added %d goodies to the shops now\n", i, delivers[i].how_many_goodies_were_added);
    }
    printf("\n");
    return 0;
}

int mutexes_initialization() {
    int i;
    for(i = 0; i < SHOPS_COUNT; i++) {
        /* NULL as attr means this is default - fastmutex */
        pthread_mutex_init(&mutexes[i], NULL);
    }
    return 0;
}

int mutexes_finalization() {
    int i;
    for(i = 0; i < SHOPS_COUNT; i++) {        
        /* It does nothing in linux but checking if mutex is locked  */
        if(0 != pthread_mutex_destroy(&mutexes[i])) {
            perror("pthread_mutex_destroy failed\n");
            exit(EXIT_FAILURE);
        }
    }
    return 0;
}

void* thread_client(void* pdata) {
    int self_id = *(int*)pdata;
    printf("I'm client %d and I was created at %ld\n\n", self_id, time(NULL));
    int i;
    int how_many_was_taken = 0;
    while(1) {
        for(i = 0; i < SHOPS_COUNT; i++) {
            /* Try to come into shop with index i */
            printf("Client %d trying to come in the shop %d\n", self_id, i);
            /* Zero if it is opened */
            if(0 == pthread_mutex_trylock(&mutexes[i])) {
                printf("Client %d came in the shop %d\n", self_id, i);
                printf("Client %d needs %d goodies\n", self_id, clients[self_id].how_many_goodies_are_wanted);
                printf("In the shop %d are %d goodies\n", i, shops[i].goodies);
                {/* Begin the critical section */
                    if(shops[i].goodies > 0) {
                        /* Order to decrease goodies in the current shop */
                        if(clients[self_id].how_many_goodies_are_wanted >= shops[i].goodies) {
                            clients[self_id].how_many_goodies_are_wanted -= shops[i].goodies;
                            how_many_was_taken = shops[i].goodies;
                            shops[i].goodies = 0;
                        } else {
                            shops[i].goodies -= clients[self_id].how_many_goodies_are_wanted;
                            how_many_was_taken = clients[self_id].how_many_goodies_are_wanted;
                            clients[self_id].how_many_goodies_are_wanted = 0;
                            printf("Client %d got %d goodies from the shop %d\n",\
                                    self_id, how_many_was_taken, i);
                            printf("%d goodies still are in the shop %d\n",\
                                    shops[i].goodies, i);
                            printf("Client %d finished shopping\n\n",\
                                    self_id);
                            /* Now this client has all what they wanted */
                            /* And this client thread has done all its job */
                            /* Now just go away from this shop, and stop thread */
                            pthread_mutex_unlock(&mutexes[i]);
                            pthread_exit(0);
                        }
                        printf("Client %d got %d goodies from the shop %d\n",\
                                self_id, how_many_was_taken, i);
                        printf("%d goodies still are in the shop %d\n",\
                                shops[i].goodies, i);
                    } else {/* If no goodies in the shop now */
                        printf("Client %d has to go to the next shop\n", self_id);
                    }
                }/* End the critical section */
                pthread_mutex_unlock(&mutexes[i]);
                printf("Client %d will sleep for %d seconds\n\n", self_id, CLIENT_DREAM);
                sleep(CLIENT_DREAM);
                printf("Client %d was awakened\n", self_id);
            /* Non zero if it is busy */
            } else {
                /* therefore just go to the next shop */
                printf("Client %d didn't come in the shop %d\n", self_id, i);
                printf("So, client %d has to go to the next shop...\n\n", self_id);
                continue;
            }
        }
    }
    return (void*)0;
}

void* thread_deliver(void* pdata) {
    int self_id = *(int*)pdata;
    int i;
    int old_cancel_state;
    printf("I'm deliver %d and I was created at %ld\n", self_id, time(NULL));
    printf("I have goodies for the shops also\n\n");
    while(1) {
        for(i = 0; i < SHOPS_COUNT; i++) {
            /* Need to be uncancelable for safety using of mutex */
            if(0 != pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, &old_cancel_state)) {
                perror("deliver's pthread_setcancelstate was failed\n");
                exit(EXIT_FAILURE);
            }
            /* Try to come into shop with index i */
            printf("Deliver %d trying to come in the shop %d\n", self_id, i);
            /* Zero if it is opened */
            if(0 == pthread_mutex_trylock(&mutexes[i])) {
                printf("Deliver %d came in the shop %d\n", self_id, i);
                {/* Begin the critical section */
                    /* Order to increase value of a goodies in the current shop */
                    shops[i].goodies += DELIVER_GIFT;
                    delivers[self_id].how_many_goodies_were_added += DELIVER_GIFT;
                    printf("Deliver %d gave %d goodies for the shop %d\n",\
                            self_id, DELIVER_GIFT, i);
                    printf("%d goodies are in the shop %d now\n",\
                            shops[i].goodies, i);
                }/* End the critical section */
                pthread_mutex_unlock(&mutexes[i]);
                /* Now can be cancelable */
                if(0 != pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, &old_cancel_state)) {
                    perror("deliver's pthread_setcancelstate was failed\n");
                    exit(EXIT_FAILURE);
                }
                printf("Deliver %d will sleep for %d seconds\n\n",\
                       self_id, DELIVER_DREAM);
                sleep(DELIVER_DREAM);
                printf("Deliver %d was awakened\n",\
                       self_id);
            /* Non zero if it is busy */
            } else {
                /* therefore just go to the next shop */
                printf("Deliver %d didn't come in the shop %d\n", self_id, i);
                printf("So, deliver %d has to go to the next shop...\n\n", self_id);
                continue;
            }
        }
    }
    return (void*)0;
}
