#include "network_server.h"
#include <stdio.h>

int main(int argc, char const *argv[]){
    
    int listening_socket = network_server_init(12345);

    network_main_loop(listening_socket);

    return 0;
}
