#include "client_stub.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]){
    /*
    Inputs:
        put <key> <data> 
        get <key> 
        del <key> 
        size 
        height 
        getkeys 
        getvalues 
        quit
    Use:
        fgets()
        strtok()
        struct rtree_t *rtree_connect(const char *address_port);
        int rtree_disconnect(struct rtree_t *rtree);
        int rtree_put(struct rtree_t *rtree, struct entry_t *entry);
        struct data_t *rtree_get(struct rtree_t *rtree, char *key);
        int rtree_del(struct rtree_t *rtree, char *key);
        int rtree_size(struct rtree_t *rtree);
        int rtree_height(struct rtree_t *rtree);
        char **rtree_get_keys(struct rtree_t *rtree);
        void **rtree_get_values(struct rtree_t *rtree);

    */
    if (argc != 2){
        printf("Uso: ./tree_client <ip_servidor>:<porto_servidor>\n");
        printf("Exemplo de uso: ./tree_client 127.0.0.1:12345\n");
        return -1;
    }

    char* command;// = malloc(16);
    char* key;// = malloc(51);
    char* data;// = malloc(51);
    //const char s[2] = " ";

    //fflush(stdin);
    //fgets(input, strlen(input) + 1, stdin);
    //command = strtok(input, s);

    //TODO try to use fgets later, it doesnt block for some reason

    scanf("%s", command);
    printf("%s", command);

    if(strcmp(command, "put")){
        //TODO...
    }
    //TODO
    return 0;

}