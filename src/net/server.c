#include "net/server.h"

struct connection *sconnect(int portno)
{
    int bind_sockfd;
    struct sockaddr_in serv_addr;
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

    // Bind on port
    if (bind(ctn->listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        fprintf(stderr, "Error on binding on port %d\n", portno);
        return ctn;
    }

    // Fill out connection values
    ctn->portno = portno;

    ctn->is_server = 0;
    ctn->is_client = 1;

    return ctn;
};

int saccept(struct connection *ctn)
{
    struct sockaddr_in cli_addr;
    socklen_t clilen = sizeof(cli_addr);
    pthread_t thread_id;

    // Listen and accept conection request
    listen(ctn->listenfd, 5);

    while (
        (ctn->sockfd = accept(ctn->listenfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen))) {
        if (pthread_create(&thread_id, NULL, connection_handler, (void *)ctn) < 0) {
            fprintf(stderr, "Error on thread creation\n");
            return 1;
        }
    }

    if (ctn->sockfd < 0) {
        fprintf(stderr, "Error on accept\n");
        return 1;
    }

    ctn->is_connected = 1;

    return 0;
}

void *connection_handler(void *vctn)
{
    //Get the socket descriptor
    struct connection *ctn = (struct connection *)vctn;

    char buf[255];
    netread(buf, 255, ctn);

    printf("Received \"%s\"\n", buf);

    return 0;
}
