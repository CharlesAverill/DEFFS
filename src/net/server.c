#include "net/server.h"

struct connection *sconnect(int portno)
{
    int bind_sockfd;
    socklen_t clilen;
    struct sockaddr_in serv_addr, cli_addr;
    struct connection *ctn = malloc(sizeof(struct connection));

    init_conn(ctn);

    // Open socket for binding
    ctn->listenfd = socket(AF_INET, SOCK_STREAM, 0);
    if (ctn->listenfd < 0) {
        fprintf(stderr, "Error opening socket\n");
        return ctn;
    }

    // Prepare serv_addr for upcoming bind, connect calls
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family      = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port        = htons(portno);

    // Bind to client
    if (bind(ctn->listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        fprintf(stderr, "Error on binding");
        return ctn;
    }

    // Listen and accept conection request
    listen(ctn->listenfd, 5);
    clilen      = sizeof(cli_addr);
    ctn->sockfd = accept(ctn->listenfd, (struct sockaddr *)&cli_addr, &clilen);
    if (ctn->sockfd < 0) {
        fprintf(stderr, "Error on accept");
        return ctn;
    }

    // Fill out connection values
    ctn->portno = portno;

    ctn->is_server = 0;
    ctn->is_client = 1;

    ctn->is_connected = 1;

    return ctn;
};
