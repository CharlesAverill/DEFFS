#include "net/net.h"

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
    if (write(ctn->sockfd, buf, size) < 0) {
        fprintf(stderr, "Error writing to socket");
        return 1;
    }

    return 0;
}

int netread(char *buf, size_t size, struct connection *ctn)
{
    if (read(ctn->sockfd, buf, size) < 0) {
        fprintf(stderr, "Error reading from socket");
        return 1;
    }

    return 0;
}
