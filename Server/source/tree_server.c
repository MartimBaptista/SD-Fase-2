#include "network_server.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]){

    if (argc != 2){
        printf("Uso: ./tree_server <porto_servidor>\n");
        printf("Exemplo de uso: ./tree_server 12345\n");
        return -1;
    }

    int listening_socket = atoi(argv[1]);

    network_main_loop(listening_socket);

    return 0;
}