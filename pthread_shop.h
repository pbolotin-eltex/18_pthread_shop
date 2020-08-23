#ifndef _PTHREAD_SHOP_
#define _PTHREAD_SHOP_

#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <unistd.h>

#define SHOPS_COUNT 5

typedef struct shop {
    int goodies;
} shop;

#define CLIENTS_COUNT 3
#define CLIENT_DREAM  2 /* How long will sleep client after successful shopping */

typedef struct client {
    int how_many_goodies_are_wanted;
} client;

#define DELIVERS_COUNT 1
#define DELIVER_GIFT   500
#define DELIVER_DREAM  1  /* How long will sleep deliver after successful delivering */

typedef struct deliver {
    int how_many_goodies_were_added;
} deliver;

extern shop            shops[SHOPS_COUNT];
extern client          clients[CLIENTS_COUNT];
extern deliver         delivers[DELIVERS_COUNT];
extern pthread_mutex_t mutexes[SHOPS_COUNT];

void* thread_client(void* pdata);

void* thread_deliver(void* pdata);

int shop_initialization(shop* shops, client* clients, deliver* delivers);

int mutexes_initialization();

int mutexes_finalization();

#endif
