# pthread_shop

## Description

There are five shops with goodies.

There are tree parallel "client" threads as well.

And shop has the only one "delivery" thread.

All of them do their work.

## What was at starting point.

Each of the five shops has 500+-50 goodies.

## "Client" threads.

They were awaked at first.

Next they search shop where are goodies.

And with relation to their needs take away goodies from shop.

## 'Deliver' threads.

They were awaked after 'clients'.

They only goes to the shops and put there goodies.

## 'Main' thread

It wait when 'clients' will be satisfate by shopping.

And after that mail thread cancel delivers.

And on that point programm will be ended.

## How to compile

make