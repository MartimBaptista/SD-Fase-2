#include "network_client.h"
#include "client_stub-private.h"
#include "inet.h"

main(int argc, char const *argv[]){
    struct rtree_t *rtree = malloc(sizeof(struct rtree_t));
    rtree->server = malloc(sizeof(struct sockaddr_in));

    rtree->server->sin_family = AF_INET; // família de endereços
    rtree->server->sin_port = htons(12345); // Porta TCP
    if (inet_pton(AF_INET, "127.0.0.1", &rtree->server->sin_addr) < 1) { // Endereço IP
        printf("Erro ao converter IP\n");
        return -1;
    }

    char* answer;

    network_connect(rtree);

    /*
    rtree->message = 10;
    answer = network_send_receive(rtree, NULL);
    printf("Sent: %d; Rceived: %d\n", rtree->message, answer);

    rtree->message = 99;
    answer = network_send_receive(rtree, NULL);
    printf("Sent: %d; Rceived: %d\n", rtree->message, answer);

    rtree->message = 23;
    answer = network_send_receive(rtree, NULL);
    printf("Sent: %d; Rceived: %d\n", rtree->message, answer);

    rtree->message = 77;
    answer = network_send_receive(rtree, NULL);
    printf("Sent: %d; Rceived: %d\n", rtree->message, answer);
    */

    char *msg;
    
    msg = "yo";
    answer = network_send_receive(rtree, msg);
    printf("Sent: %s; Rceived: %s\n", msg, answer);
    free(answer);

    msg = "close";
    answer = network_send_receive(rtree, msg);
    printf("Sent: %s; Rceived: %s\n", msg, answer);
    free(answer);

    network_close(rtree);
    free(rtree->server);
    free(rtree);
}
