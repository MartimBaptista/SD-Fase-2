#include "inet.h"
#include <errno.h>
#include "client_stub-private.h"

/* Esta função deve:
 * - Obter o endereço do servidor (struct sockaddr_in) a base da
 *   informação guardada na estrutura rtree;
 * - Estabelecer a ligação com o servidor;
 * - Guardar toda a informação necessária (e.g., descritor do socket)
 *   na estrutura rtree;
 * - Retornar 0 (OK) ou -1 (erro).
 */
int network_connect(struct rtree_t *rtree){

    // Cria socket TCP e guarda-a no rtree
    if ((rtree->client_sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Erro ao criar socket TCP");
        return -1;
    }

    // Estabelece conexão com o servidor definido na estrutura server
    if (connect(rtree->client_sockfd, rtree->server, sizeof(*rtree->server)) < 0) {
        perror("Erro ao conectar-se ao servidor");
        close(rtree->client_sockfd);
        return -1;
    }

    return 0;
}

/* Função para ler toda uma messagem num determinado porto.
 * Retornar o tamanho total lido (OK) ou <0 (erro).
 */
int read_all(int sock, char *buf, int len){
    int bufsize = len;
    while(len>0) {
        int res = read(sock, buf, len);
        if(res<0) {
            if(errno==EINTR) continue;
            perror("read failed:");
            return res;
        }
    buf += res;
    len -= res;
    }
    return bufsize;
}

/* Função para escrever toda uma messagem num determinado porto.
 * Retornar o tamanho total escrito (OK) ou <0 (erro).
 */
int write_all(int sock, char *buf, int len){
    int bufsize = len;
    while(len>0) {
        int res = write(sock, buf, len);
        if(res<0) {
            if(errno==EINTR) continue;
            perror("write failed:");
            return res;
        }
    buf += res;
    len -= res;
    }
    return bufsize;
}

/* Esta função deve:
 * - Obter o descritor da ligação (socket) da estrutura rtree_t;
 * - Serializar a mensagem contida em msg;
 * - Enviar a mensagem serializada para o servidor;
 * - Esperar a resposta do servidor;
 * - De-serializar a mensagem de resposta;
 * - Retornar a mensagem de-serializada ou NULL em caso de erro.
 */
struct message_t *network_send_receive(struct rtree_t * rtree, struct message_t *msg){

    int nbytes, answer;

    // Envia o int
    printf("Args: %d, %d, %d\n", rtree->client_sockfd, rtree->message, sizeof(int)); //TODO remove this
    if((nbytes = write(rtree->client_sockfd, &rtree->message, sizeof(int))) != sizeof(int)){
        perror("Erro ao enviar dados ao servidor");
        close(rtree->client_sockfd);
        return -1;
    }

    printf("À espera de resposta do servidor ...\n");

    // Recebe tamanho da string
    if((nbytes = read(rtree->client_sockfd, &answer, sizeof(int))) != sizeof(int)){
        perror("Erro ao receber dados do servidor");
        close(rtree->client_sockfd);
        return -1;
    };

    return answer;
}

/* A função network_close() fecha a ligação estabelecida por
 * network_connect().
 */
int network_close(struct rtree_t * rtree){
    close(rtree->client_sockfd);
    return 0;
}