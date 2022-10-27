#include "client_stub.h"
#include "client_stub-private.h"
#include "entry.h"
#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUFFERSIZE 500 //max input size

int main(int argc, char *argv[]){
    //Checking arguments
    if (argc != 2){
        printf("Uso: ./tree_client <ip_servidor>:<porto_servidor>\n");
        printf("Exemplo de uso: ./tree_client 127.0.0.1:12345\n");
        return -1;
    }

    //Connecting to the server/tree
    struct rtree_t *rtree;
    
    if(rtree = rtree_connect(argv[1]) == NULL){
        return(-1);
    }

    printf("Connected to server.");

    const char s[2] = " ";
    const char f[2] = "\0";
    while (1){
        char input[BUFFERSIZE];
        char *command, *key_s, *data_s;

        //Obtaining input
        fflush(stdin);
        fgets(input, BUFFERSIZE, stdin);
        //Removing newline from string (\n)
        input[strcspn(input, "\n")] = 0;

        //Spliting Input into its components
        command = strtok(input, s);
        key_s = strtok(NULL, s);
        data_s = strtok(NULL, f);

        //Case for each command
        if(strcmp(command, "put") == 0){

            //Checking for key
            if(key_s == NULL){
                printf("Missing key and data: \"put <key> <data>\"\n");
                continue;
            }

            //Checking for data
            if(data_s == NULL){
                printf("Missing data: \"put <key> <data>\"\n");
                continue;
            }

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
            /*
            if(rtree_put(rtree, entry) < 0){
                perror("Error on put:");
                continue;
            }
            */
            entry_destroy(entry);

        }
        else if(strcmp(command, "get") == 0){
            //Checking for key
            if(key_s == NULL){
                printf("Missing key: \"get <key>\"\n");
                continue;
            }

            //Building key
            char* key = malloc(strlen(key_s) + 1);
            strcpy(key, key_s);

            //Getting it from the tree
            struct data_t *data;
            /*
            if(data = rtree_get(rtree, key) < 0){
                perror("Error on get:");
                continue;
            }
            printf("Received data: %s", (char*)data->data);
            */
            free(key);
        }
        else if(strcmp(command, "del") == 0){
            //Checking for key
            if(key_s == NULL){
                printf("Missing key: \"del <key>\"\n");
                continue;
            }

            //Building key
            char* key = malloc(strlen(key_s) + 1);
            strcpy(key, key_s);

            //Delliting it from the tree
            /*
            if(rtree_del(rtree, key) < 0){
                perror("Error on del:");
                continue;
            }
            */
           free(key);
        }
        else if(strcmp(command, "size") == 0){
            //Getting tree size
            //print("Tree size: %d", rtree_size(rtree));
        }
        else if(strcmp(command, "height") == 0){
            //Getting tree height
            //print("Tree height: %d", rtree_height(rtree));
        }
        else if(strcmp(command, "getkeys") == 0){
            //TODO...
            //char **rtree_get_keys(struct rtree_t *rtree);

        }
        else if(strcmp(command, "getvalues") == 0){
            //TODO...
            //void **rtree_get_values(struct rtree_t *rtree);

        }
        else if(strcmp(command, "quit") == 0){
            //Disconnecting from server/tree
            /*
            if(rtree_disconnect(rtree) < 0){
                perror("Error on disconnect:");
                return -1;
            }
            */
            printf("Disconnected from server\n");
            break;
        }
        else if(strcmp(command, "commands") == 0){
            printf("\n------//-----\n put <key> <data>\n get <key> \n del <key> \n size \n height \n getkeys \n getvalues \n quit \n------//-----\n");
        }
        else{
            printf("Input not recognised, type \"commands\" for a list of the possible commands.\n");
        }
    }
    return 0;
}