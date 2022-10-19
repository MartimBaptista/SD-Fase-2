



#include "serialization.h"
#include "data.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* Serializa todas as keys presentes no array de strings keys
 * para o buffer keys_buf que será alocado dentro da função.
 * O array de keys a passar em argumento pode ser obtido através 
 * da função tree_get_keys. Para além disso, retorna o tamanho do
 * buffer alocado ou -1 em caso de erro.
 */
int keyArray_to_buffer(char **keys, char **keys_buf) {
    
    size_t total_buf_size = 0;
    for (size_t i = 0; keys[i] != NULL; i++)
    {
        total_buf_size += strlen(keys[i]) + sizeof(int);

        // char str[10];
        // sprintf(str, "%d", i);        
        // puts(str);
    }
    
    keys_buf[0] = malloc(total_buf_size);

    size_t buf_size = 0;
    size_t i = 0;
    for (i = 0; keys[i] != NULL; i++)
    {

        int size = strlen(keys[i]);

        char *key = keys[i];

        memcpy(&keys_buf[0][buf_size], &size, sizeof(size));
        memcpy(&keys_buf[0][buf_size + sizeof(size)], key, size);

        buf_size += sizeof(size) + size;
    }

    return buf_size;
}

/* De-serializa a mensagem contida em keys_buf, com tamanho
 * keys_buf_size, colocando-a e retornando-a num array char**,
 * cujo espaco em memória deve ser reservado. Devolve NULL
 * em caso de erro.
 */
char** buffer_to_keyArray(char *keys_buf, int keys_buf_size) {

    int sizeRead = 0;

    char **keys = malloc(sizeof(char *) * 2);

    int cnt = 0;

    while (sizeRead < keys_buf_size) {

        int size = 0;

        memcpy(&size, &keys_buf[sizeRead], sizeof(int));

        sizeRead +=  sizeof(size) + size;
        cnt++;
    }

    int i = 0;
    sizeRead = 0;
    while (i < cnt) {
        int size = 0;

        memcpy(&size, &keys_buf[sizeRead], sizeof(size));

        keys[i] = malloc(size + 1);
        memcpy(keys[i], &keys_buf[sizeRead + sizeof(size)], size);
        keys[i][size] = '\0'; 

        puts(keys[i]);

        sizeRead +=  sizeof(size) + size;
        i++;
    }

    return keys;
}
