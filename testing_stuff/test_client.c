#include "network_client.h"
#include "client_stub-private.h"

main(int argc, char const *argv[]){
    struct rtree_t *rtree = malloc(sizeof(struct rtree_t));
    rtree->server = malloc(sizeof(struct sockaddr_in));

    server.sin_family = AF_INET; // família de endereços
    server.sin_port = htons(atoi(argv[2])); // Porta TCP
    if (inet_pton(AF_INET, &rtree->server.sin_addr, &rtree->server.sin_addr) < 1) { // Endereço IP
        printf("Erro ao converter IP\n");
        close(sockfd);
        return -1;
    }
}
