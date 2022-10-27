#include "client_stub.h"
#include "client_stub-private.h"

/* Remote tree. A definir pelo grupo em client_stub-private.h
 */
struct rtree_t;

/* Função para estabelecer uma associação entre o cliente e o servidor, 
 * em que address_port é uma string no formato <hostname>:<port>.
 * Retorna NULL em caso de erro.
 */
struct rtree_t *rtree_connect(const char *address_port) {
    struct rtree_t *rtree = malloc(sizeof(rtree));

    rtree->server->sin_addr.s_addr = strtok(address_port, ":");
    rtree->server->sin_port = strtok(NULL, ":");
    
    return network_connect(rtree) == 0? rtree : NULL;
}

/* Termina a associação entre o cliente e o servidor, fechando a 
 * ligação com o servidor e libertando toda a memória local.
 * Retorna 0 se tudo correr bem e -1 em caso de erro.
 */
int rtree_disconnect(struct rtree_t *rtree) {
    int resp = network_close(rtree);
    free(rtree);
    return resp;
}

/* Função para adicionar um elemento na árvore.
 * Se a key já existe, vai substituir essa entrada pelos novos dados.
 * Devolve 0 (ok, em adição/substituição) ou -1 (problemas).
 */
int rtree_put(struct rtree_t *rtree, struct entry_t *entry) {
    MessageT *msg;

    message_t__init(msg);

    // write codes to message
    msg->opcode = MESSAGE_T__OPCODE__OP_PUT;
    msg->c_type = MESSAGE_T__C_TYPE__CT_ENTRY;
    msg->entry->key = entry->key;

    msg->entry->data.len = entry->value->datasize;
    memcpy(msg->entry->data.data, entry->value->data, entry->value->datasize);

    MessageT *answer = network_send_receive(rtree, msg);

    if (answer->opcode == MESSAGE_T__OPCODE__OP_PUT + 1 && answer->c_type == MESSAGE_T__C_TYPE__CT_NONE)
    {
        return 0;
    }
    
    return -1; 
}

/* Função para obter um elemento da árvore.
 * Em caso de erro, devolve NULL.
 */
struct data_t *rtree_get(struct rtree_t *rtree, char *key) {
    MessageT *msg;

    message_t__init(msg);

    // write codes to message
    msg->opcode = MESSAGE_T__OPCODE__OP_GET;
    msg->c_type = MESSAGE_T__C_TYPE__CT_KEY;

    msg->entry->key = key;

    MessageT *answer = network_send_receive(rtree, msg);

    if (answer->opcode == MESSAGE_T__OPCODE__OP_GET + 1 && answer->c_type == MESSAGE_T__C_TYPE__CT_VALUE)
    {
        struct data_t *ret = data_create(answer->entry->data.len);

        memcpy(ret->data, answer->entry->data.data, ret->datasize);     

        return ret;
    }
    
    return NULL; 
}

/* Função para remover um elemento da árvore. Vai libertar 
 * toda a memoria alocada na respetiva operação rtree_put().
 * Devolve: 0 (ok), -1 (key not found ou problemas).
 */
int rtree_del(struct rtree_t *rtree, char *key) {
    MessageT *msg;

    message_t__init(msg);

    // write codes to message
    msg->opcode = MESSAGE_T__OPCODE__OP_DEL;
    msg->c_type = MESSAGE_T__C_TYPE__CT_KEY;

    strcpy(msg->entry->key, key);

    MessageT *answer = network_send_receive(rtree, msg);

    if (answer->opcode == MESSAGE_T__OPCODE__OP_DEL + 1 && answer->c_type == MESSAGE_T__C_TYPE__CT_NONE)
    {
        return 0;
    }
    
    return -1; 
}

/* Devolve o número de elementos contidos na árvore.
 */
int rtree_size(struct rtree_t *rtree) {
    MessageT *msg;

    message_t__init(msg);

    // write codes to message
    msg->opcode = MESSAGE_T__OPCODE__OP_SIZE;
    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;

    // send message
    MessageT *answer = network_send_receive(rtree, msg);

    if (answer->opcode == MESSAGE_T__OPCODE__OP_SIZE + 1 && answer->c_type == MESSAGE_T__C_TYPE__CT_RESULT)
    {
        return answer->size;
    }
    
    return -1;
}

/* Função que devolve a altura da árvore.
 */
int rtree_height(struct rtree_t *rtree) {
    MessageT *msg;

    message_t__init(msg);

    // write codes to message
    msg->opcode = MESSAGE_T__OPCODE__OP_HEIGHT;
    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;

    // send message
    MessageT *answer = network_send_receive(rtree, msg);

    if (answer->opcode == MESSAGE_T__OPCODE__OP_HEIGHT + 1 && answer->c_type == MESSAGE_T__C_TYPE__CT_RESULT)
    {
        return answer->size;
    }
    
    return -1; 
}

/* Devolve um array de char* com a cópia de todas as keys da árvore,
 * colocando um último elemento a NULL.
 */
char **rtree_get_keys(struct rtree_t *rtree) {
    MessageT *msg;

    message_t__init(msg);

    // write codes to message
    msg->opcode = MESSAGE_T__OPCODE__OP_GETKEYS;
    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;

    MessageT *answer = network_send_receive(rtree, msg);

    if (answer->opcode != MESSAGE_T__OPCODE__OP_GETKEYS + 1 || answer->c_type != MESSAGE_T__C_TYPE__CT_KEYS)
    {
        return NULL;
    }
    
    int n_keys = answer->n_keys;
    char **ret;

    ret = malloc(sizeof(char *) * n_keys);

    for (size_t i = 0; i < n_keys; i++) 
    {
        ret[i] = malloc(strlen(answer->keys[i]) + 1);
        strcpy(ret[i], answer->keys[i]);
    }

    return ret;    
}

/* Devolve um array de void* com a cópia de todas os values da árvore,
 * colocando um último elemento a NULL.
 */
void **rtree_get_values(struct rtree_t *rtree) {
    MessageT *msg;

    message_t__init(msg);

    // write codes to message
    msg->opcode = MESSAGE_T__OPCODE__OP_GETVALUES;
    msg->c_type = MESSAGE_T__C_TYPE__CT_NONE;

    MessageT *answer = network_send_receive(rtree, msg);

    if (answer->opcode != MESSAGE_T__OPCODE__OP_GETVALUES + 1 || answer->c_type != MESSAGE_T__C_TYPE__CT_VALUES)
    {
        return NULL;
    }
    
    int n_values = answer->n_values;
    
    struct data_t **ret = malloc(sizeof(struct data_t *) * (n_values + 1));

    for (size_t i = 0; i < n_values; i++) 
    {
        
        ret[i] = malloc(sizeof(struct data_t));

        
        ret[i]->datasize = answer->values[i].len;
        
        ret[i]->data = malloc(ret[i]->datasize);
        memcpy(ret[i]->data, answer->values[i].data, ret[i]->datasize);
    }

    return ret;
}