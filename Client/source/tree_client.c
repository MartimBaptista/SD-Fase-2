#include "client_stub.h"
#include "client_stub-private.h"
#include "entry.h"
#include "data.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

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
    
    if((rtree = rtree_connect(argv[1])) == NULL){
        return(-1);
    }

    printf("Connected to server.\n");

    const char s[2] = " ";
    const char f[2] = "\0";
    while (1){
        char input[BUFFERSIZE];
        char *command, *key_s, *data_s;
        printf("\n>");

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
        //CASE PUT:
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
            if(rtree_put(rtree, entry) < 0){
                perror("Error on put");
                continue;
            }

            printf("Entry has been put into the tree.\n");

            entry_destroy(entry);
        }

        //CASE GET:
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
            if((data = rtree_get(rtree, key)) == NULL){
                //In case there is an entry with that key
                if(strcmp(strerror(errno), "Success") == 0){
                    printf("Key not found on tree.\n");
                    continue;
                }
                //In case there isn't an entry with that key
                else{
                perror("Error on get");
                continue;
                }
            }
            printf("Key %s has the following data: %s\n", key, (char*)data->data);
            free(key);
            data_destroy(data);
        }

        //CASE DEL:
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
            if(rtree_del(rtree, key) < 0){
                //In case there is an entry with that key
                if(strcmp(strerror(errno), "Success") == 0){
                    printf("Key not found on tree.\n");
                    continue;
                }
                //In case there isn't an entry with that key
                else{
                    perror("Error on del");
                    continue;
                }
            }
            printf("Entry deleted from tree.\n");
            free(key);
        }

        //CASE SIZE:
        else if(strcmp(command, "size") == 0){
            //Getting tree size
            printf("Tree size: %d", rtree_size(rtree));
        }

        //CASE HEIGHT:
        else if(strcmp(command, "height") == 0){
            //Getting tree height
            printf("Tree height: %d", rtree_height(rtree));
        }

        //CASE GETKEYS:
        else if(strcmp(command, "getkeys") == 0){
            char **keys = rtree_get_keys(rtree);

            printf("Keys of Tree: {");
            for (size_t i = 0; keys[i] != NULL; i++){
                printf("%s", keys[i]);
                if (keys[i + 1] != NULL){
                    printf(", ");
                }
            }
            printf("}\n");
        }

        //CASE GETVALUES:
        else if(strcmp(command, "getvalues") == 0){
            void** datas = rtree_get_values(rtree);

            int i = 0;
            printf("Values of Tree: {");
            while(datas[i] != NULL){
                struct data_t* data = (struct data*)datas[i];
                printf("%s", data->data);
                if (datas[i + 1] != NULL){
                    printf(", ");
                }
                i++;
            }
            printf("}\n");
        }

        //CASE QUIT:
        else if(strcmp(command, "quit") == 0){
            //Disconnecting from server/tree
            if(rtree_disconnect(rtree) < 0){
                perror("Error on disconnect");
                return -1;
            }
            printf("Disconnected from server\n");
            break;
        }

        //CASE COMMANDS:
        else if(strcmp(command, "commands") == 0){
            printf("------//-----\n put <key> <data>\n get <key> \n del <key> \n size \n height \n getkeys \n getvalues \n quit \n------//-----\n");
        }

        //CASE DEFAULT
        else{
            printf("Input not recognised, type \"commands\" for a list of the possible commands.\n");
        }
    }
    return 0;
}