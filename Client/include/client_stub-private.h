#include "inet.h"

struct rtree_t{
    struct sockaddr_in *server;
    int client_sockfd;
};