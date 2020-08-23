# Make

.PHONY = clean all
KEYS :=
LIBS := -lpthread
CC := gcc

all : pthread_shop

pthread_shop : pthread_shop.o pthread_shop_main.o
	$(CC) $(KEYS) pthread_shop.o pthread_shop_main.o -o pthread_shop $(LIBS)

pthread_shop.o : pthread_shop.c pthread_shop.h
	$(CC) $(KEYS) -c pthread_shop.c -o pthread_shop.o $(LIBS)

pthread_shop_main.o : pthread_shop_main.c pthread_shop.h
	$(CC) $(KEYS) -c pthread_shop_main.c -o pthread_shop_main.o $(LIBS)

clean :
	rm -f *.o *.gch pthread_shop
