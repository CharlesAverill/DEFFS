#ifndef NET_H
#define NET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

typedef struct connection {
    char addr[24];
    int portno;
    int sockfd;
    int listenfd;
    int connect_retries;
    
    int is_connected;
    
    int is_server;
    int is_client;
} connection;

void init_conn(struct connection *ctn);
int close_conn(struct connection *ctn);

int netwrite(char *buf, size_t size, struct connection *ctn);
int netread(char *buf, size_t size, struct connection *ctn);

#endif
