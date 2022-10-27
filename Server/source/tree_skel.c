#include "sdmessage.pb-c.h"
#include "tree.h"
#include <stdlib.h>
#include <string.h>
#include "message.h"

struct tree_t *tree;


/* Inicia o skeleton da árvore.
 * O main() do servidor deve chamar esta função antes de poder usar a
 * função invoke(). 
 * Retorna 0 (OK) ou -1 (erro, por exemplo OUT OF MEMORY)
 */
int tree_skel_init(){
    tree = tree_create();

    if(tree == NULL)
        return -1;

    return 0;
}

/* Liberta toda a memória e recursos alocados pela função tree_skel_init.
 */
void tree_skel_destroy(){
    tree_destroy(tree);
}

/* Executa uma operação na árvore (indicada pelo opcode contido em msg)
 * e utiliza a mesma estrutura message_t para devolver o resultado.
 * Retorna 0 (OK) ou -1 (erro, por exemplo, árvore nao incializada)
*/
int invoke(struct message_t *msg) {

MessageT__Opcode op = msg->message.opcode;

int size;
char * key;
struct data_t* data;


switch(op) {
    case MESSAGE_T__OPCODE__OP_SIZE: ;

        msg->message.opcode = MESSAGE_T__OPCODE__OP_SIZE + 1;
        msg->message.c_type = MESSAGE_T__C_TYPE__CT_RESULT;
        msg->message.size = tree_size(tree);
        return 0;

        break;

    case MESSAGE_T__OPCODE__OP_HEIGHT: ;

        msg->message.size = tree_height(tree);
        msg->message.opcode = MESSAGE_T__OPCODE__OP_HEIGHT + 1;
        msg->message.c_type = MESSAGE_T__C_TYPE__CT_RESULT;
        return 0;
        break;

    case MESSAGE_T__OPCODE__OP_DEL: ;

        //executa tree_del
        int del = tree_del(tree, msg->message.entry->key);

        //caso de erro em tree_del
        if(del == -1){
          msg->message.opcode = MESSAGE_T__OPCODE__OP_ERROR;
          msg->message.c_type = MESSAGE_T__C_TYPE__CT_NONE;
          return 0;
        }

        msg->message.opcode = MESSAGE_T__OPCODE__OP_DEL + 1;
        msg->message.c_type = MESSAGE_T__C_TYPE__CT_NONE;
        return 0;
        break;

    case MESSAGE_T__OPCODE__OP_GET: ;

        data = tree_get(tree, msg->message.entry->key);

        //caso a key nao esteja presente
        if(data == NULL){
            msg->message.opcode = MESSAGE_T__OPCODE__OP_GET + 1;
            msg->message.c_type = MESSAGE_T__C_TYPE__CT_NONE;
            msg->message.entry->data.data = NULL;
            msg->message.entry->data.len = 0;
            return 0;
        }

        msg->message.opcode = MESSAGE_T__OPCODE__OP_GET + 1;
        msg->message.c_type = MESSAGE_T__C_TYPE__CT_VALUE;
        msg->message.entry->data.data = data->data;
        msg->message.entry->data.len = data->datasize;
        return 0;
        break;

    case MESSAGE_T__OPCODE__OP_PUT: ;

        //cria data para tree_put
        key = msg->message.entry->key;
        size = msg->message.size;
        data = data_create2(size, msg->message.entry->data.data);

        //caso de erro em tree_put
        if(tree_put(tree,key,data) == -1 ){
          msg->message.opcode = MESSAGE_T__OPCODE__OP_ERROR;
          msg->message.c_type = MESSAGE_T__C_TYPE__CT_NONE;
          return 0;
        }

        msg->message.opcode = MESSAGE_T__OPCODE__OP_PUT + 1;
        msg->message.c_type = MESSAGE_T__C_TYPE__CT_NONE;
        return 0;
        break;

    case MESSAGE_T__OPCODE__OP_GETKEYS: ;

        char** keys = tree_get_keys(tree);

        //caso arvore vazia
        if(keys == NULL){
            msg->message.opcode = MESSAGE_T__OPCODE__OP_BAD;
            msg->message.c_type = MESSAGE_T__C_TYPE__CT_BAD;
            return 0;
        }

        msg->message.opcode = MESSAGE_T__OPCODE__OP_GETKEYS + 1;
        msg->message.c_type = MESSAGE_T__C_TYPE__CT_KEYS;
        msg->message.keys = keys;
        msg->message.size = tree_size(tree);
        return 0;
        break;

    case MESSAGE_T__OPCODE__OP_GETVALUES: ;

        void** values = tree_get_values(tree);

        //caso arvore vazia
        if(values == NULL){
            msg->message.opcode = MESSAGE_T__OPCODE__OP_BAD;
            msg->message.c_type = MESSAGE_T__C_TYPE__CT_BAD;
            return 0;
        }

        int i = 0;
        while(values[i] != NULL){
            ProtobufCBinaryData data_temp;
            data_temp.len = sizeof(values[i]);
            data_temp.data = malloc(sizeof(values[i]));
            memcpy(data_temp.data, values[i], sizeof(void*));
            msg->message.values[i] = data_temp;
            i++;
        }

        msg->message.opcode = MESSAGE_T__OPCODE__OP_GETVALUES + 1;
        msg->message.c_type = MESSAGE_T__C_TYPE__CT_VALUES;
        msg->message.n_values = i;

        return 0;
        break;

    case MESSAGE_T__OPCODE__OP_ERROR: ;
       return -1;
    default: ;
       return -1;
    }   

    return -1;
};
