#ifndef CLIENT_H
#define CLIENT_H

#include "net/net.h"

struct connection *cconnect(char *addr, int portno, int connect_retries);

#endif
