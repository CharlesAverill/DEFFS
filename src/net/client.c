#include "net/client.h"

struct connection *cconnect(char *addr, int portno, int connect_retries)
{
    struct sockaddr_in serv_addr;
    struct hostent *server;
    struct connection *ctn = malloc(sizeof(struct connection));

    init_conn(ctn);

    // Open socket
    ctn->sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (ctn->sockfd < 0) {
        fprintf(stderr, "Error opening socket\n");
        return ctn;
    }

    // Find server by its IP address
    server = gethostbyname(addr);
    if (server == NULL) {
        fprintf(stderr, "Error, no such host %s\n", addr);
        return ctn;
    }

    // Prepare serv_addr for upcoming connect call
    bzero((char *)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
    serv_addr.sin_port = htons(portno);

    // Connect to server
    int connected = 0;
    int timeout   = 1;
    for (int i = 0; i < connect_retries || connect_retries == -1; i++) {
        if (connect(ctn->sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
            connected = 1;
            break;
        }

        // Exponential backoff
        sleep(timeout);
        timeout *= 2;
    }
    if (connected != 1) {
        fprintf(stderr, "Error connecting to host %s on port %d\n", addr, portno);
        return ctn;
    }

    // Fill out connection values
    strcpy(ctn->addr, addr);
    ctn->portno          = portno;
    ctn->connect_retries = connect_retries;

    ctn->is_server = 1;
    ctn->is_client = 0;

    ctn->is_connected = 1;

    return ctn;
}
