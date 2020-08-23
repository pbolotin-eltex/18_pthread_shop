# pthread_shop

## program structure

```
pthread_shop.h  -  header with all types and prototypes of functions

pthread_shop.c  -  implementation of all functions but main.
                   and global variables are here!

pthread_shop_main.c  -  implementation of main and using global arrays.
```

## Description

There are five shops with goodies.

There are tree parallel "client" threads as well.

And shop has the only one "delivery" thread.

All of them do their work.

## What is at the starting point.

Each of the five shops has 500+-50 goodies.

Each of the client-threads has needs for 10000 +- 200 goodies.

## "Client" threads.

They were created.

Next they search the shop where goodies are.

And with relation to their needs they take away goodies from shop.

## 'Deliver' threads.

They were created after 'clients'.

They just goes to the shops and put there goodies each time.

## 'Main' thread

It wait when 'clients' will be satisfated by shopping.

And after that main-thread cancel deliver-threads.

And on that point programm will be ended.

## How to compile

make

## How to use

./pthread_shop

or if you need to analize full output:

./pthread_shop > log.txt
