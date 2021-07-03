#include "net/net.h"

/*
* FILENAME: net.c
*
* DESCRIPTION: Generic TCP operations, used in conjunction with
*              client.c and server.c
*
* USAGE: 
*        // CLIENT CODE
*        struct connection *host_connection;
*        host_connection = cconnect("<ip address>", <port>, 5);
*
*        char rbuf[255] = "Hello from the client!";
*        netwrite(rbuf, 255, host_connection);
*
*        bzero(rbuf, 255);
*        netread(rbuf, 255, host_connection);
*
*        char *message_chunks[(strlen(rbuf) / 3) + 1];
*        split_into_shards(rbuf, message_chunks, 3);
*
*        printf("Rbuf: %s Len: %zu\n", rbuf, strlen(rbuf));
*
*        for (int i = 0; i < 3; i++) {
*            printf("Shard %d: %s\n", i, message_chunks[i]);
*        }
*
*        close_conn(host_connection);
*        
*        // NETWORK CODE
*        struct connection *client_connection;
*        client_connection = sconnect(<port>);
*        saccept(client_connection);
*        
*        char wbuf[255];
*        bzero(wbuf, 255);
* 
*        netread(wbuf, 255, client_connection);
*        printf("Got %s from the client\n", wbuf);
* 
*        netwrite("Hey there client!\n", 255, client_connection);
* 
*        close_conn(client_connection);
*
* AUTHOR: Charles Averill
*/

void init_conn(struct connection *ctn)
{
    bzero(ctn->addr, sizeof(ctn->addr));
    ctn->portno          = -1;
    ctn->sockfd          = -1;
    ctn->listenfd        = -1;
    ctn->connect_retries = 1;

    ctn->is_connected = 0;

    ctn->is_server = 0;
    ctn->is_client = 0;
}

int close_conn(struct connection *ctn)
{
    close(ctn->sockfd);
    if (ctn->is_server) {
        close(ctn->listenfd);
    }

    ctn->is_connected = 0;

    ctn->is_server = 0;
    ctn->is_client = 0;

    free(ctn->addr);
}

int netwrite(char *buf, size_t size, struct connection *ctn)
{
    ssize_t total_bytes_written = 0;
    while (total_bytes_written != size) {
        ssize_t bytes_written =
            write(ctn->sockfd, &buf[total_bytes_written], size - total_bytes_written);
        if (bytes_written < 0) {
            fprintf(stderr, "Error writing to socket\n");
            return 1;
        }
        total_bytes_written += bytes_written;
    }

    return 0;
}

int netread(char *buf, size_t size, struct connection *ctn)
{
    if (read(ctn->sockfd, buf, size) < 0) {
        fprintf(stderr, "Error reading from socket\n");
        return 1;
    }

    return 0;
}
