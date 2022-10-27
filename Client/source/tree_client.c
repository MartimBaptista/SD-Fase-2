#include "client_stub.h"
#include "client_stub-private.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 500

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

    struct rtree_t *rtree = rtree_connect(argv[1]);

    int running = 1;
    char input[BUFFERSIZE];
    char *command, *key, *data;
    const char s[2] = " ";
    const char f[2] = "\0";
    while (running){
        char input[BUFFERSIZE];
        char *command, *key_s, *data_s;

        //Obtaining input
        fflush(stdin);
        fgets(input, BUFFERSIZE, stdin);

        //Spliting Input into its components
        command = strtok(input, s);
        printf("Command: %s\n", command); //TODO remove this
        key_s = strtok(NULL, s);
        printf("Key: %s\n", key_s); //TODO remove this
        data_s = strtok(NULL, f);
        printf("Data: %s\n", data_s); //TODO remove this

        //Chcking the input and calling tree_stub

        if(strcmp(command, "put") == 0){
            //Checking for key
            if(key == NULL){
                printf("Missing key.\n");
                continue;
            }

            //Checking for data
            if(key == NULL){
                printf("Missing data.\n");
                continue;
            }

            //Building data
            struct data_t *data;
            data->datasize = strlen(data_s) + 1;
            strcpy(data->data, data_s);

            //Building entry
            struct entry_t *entry;
            strcpy(entry->key, key_s);
            entry->value = data;

            //Putting in tree
            if(rtree_put(rtree, entry) < 0){
                perror("Error on put:");
                break;
            }
        }
        else if(strcmp(command, "get") == 0){
            //Checking for key
            if(key == NULL){
                printf("Missing key.\n");
                continue;
            }

        struct data_t *data = rtree_get(rtree, key_s);
        //CONTINUE HERE!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!

        }
        else if(strcmp(command, "del") == 0){
            //TODO...
        }
        else if(strcmp(command, "size") == 0){
            //TODO...
        }
        else if(strcmp(command, "height") == 0){
            //TODO...
        }
        else if(strcmp(command, "getkeys") == 0){
            //TODO...
        }
        else if(strcmp(command, "getvalues") == 0){
            //TODO...
        }
        else if(strcmp(command, "quit") == 0){
            if(rtree_disconnect(rtree) == 0){

            }
            printf("TODO");
        }
        else{
            printf("what?\n");
        }
    }
    //TODO
    return 0;

}