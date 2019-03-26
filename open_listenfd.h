#ifndef OPEN_LISTENFD_H
#define OPEN_LISTENFD_H

#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#define BUF_LEN 512

int open_listenfd(int port);

#endif
