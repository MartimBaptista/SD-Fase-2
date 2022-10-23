#include "network_server.h"

main(int argc, char const *argv[]){
    
    int listening_socket = network_server_init(12345);

    network_main_loop(listening_socket);
}
