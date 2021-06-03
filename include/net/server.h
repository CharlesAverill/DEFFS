#ifndef SERVER_H
#define SERVER_H

#include "net/net.h"

struct connection *sconnect(int portno);
int saccept(struct connection *ctn);

#endif
