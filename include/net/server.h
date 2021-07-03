#ifndef SERVER_H
#define SERVER_H

#include <pthread.h>

#include "net/net.h"
#include "deffs.h"

struct connection *sconnect(int portno);
int saccept(struct connection *ctn);
void *connection_handler(void *vctn);

#endif
