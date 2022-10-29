#include "client_stub.h"
#include "client_stub-private.h"
#include "entry.h"
#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define BUFFERSIZE 500 //max input size

void put( struct rtree_t *rtree, char *key_s, char *data_s);


int main(int argc, char *argv[]){
    //Checking arguments
    if (argc != 2){
        printf("Uso: ./tree_client <ip_servidor>:<porto_servidor>\n");
        printf("Exemplo de uso: ./tree_client 127.0.0.1:12345\n");
        
        argv[1] = "127.0.0.1:12345";
    }

    //Connecting to the server/tree
    struct rtree_t *rtree;
    
    if((rtree = rtree_connect(argv[1])) == NULL){
        return(-1);
    }

    printf("Connected to server.\n");

    //Checking for key
    char *key_s, *data_s;

    key_s = "1";
    data_s = "hey";
    put(rtree, key_s, data_s);

    key_s = "2";
    data_s = "hey";
    put(rtree, key_s, data_s);

    key_s = "0";
    data_s = "hey";
    put(rtree, key_s, data_s);

    key_s = "4";
    data_s = "hey";
    put(rtree, key_s, data_s);

    key_s = "3";
    data_s = "hey";
    put(rtree, key_s, data_s);


}

void put( struct rtree_t *rtree, char *key_s, char *data_s) {
    //Building data
    int data_size = strlen(data_s) + 1;
    void *buffer = malloc(data_size);
    memcpy(buffer, data_s, data_size);
    
    struct data_t *data = data_create2(data_size, buffer);

    //Building entry
    char* key = malloc(strlen(key_s) + 1);
    strcpy(key, key_s);
    struct entry_t *entry = entry_create(key, data);

    //Putting it in tree
    if(rtree_put(rtree, entry) < 0){
        perror("Error on put");
        return;
    }

    printf("Entry has been put into the tree.\n");

    entry_destroy(entry);
}

void size(struct rtree_t *rtree) {
    printf("Tree size: %d", rtree_size(rtree));
}
