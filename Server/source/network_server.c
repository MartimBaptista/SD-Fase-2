#include "inet.h"
#include <errno.h>
#include "sdmessage.pb-c.h"

/* Função para preparar uma socket de receção de pedidos de ligação
 * num determinado porto.
 * Retornar descritor do socket (OK) ou -1 (erro).
 */
int network_server_init(short port){
    int listening_socket;
    struct sockaddr_in server;

    // Cria socket TCP
    if ((listening_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
        perror("Erro ao criar socket");
        return -1;
    }

    // Preenche estrutura server com endereço(s) para associar (bind) à socket 
    server.sin_family = AF_INET;
    server.sin_port = htons(port); // Porta TCP
    server.sin_addr.s_addr = htonl(INADDR_ANY); // Todos os endereços na máquina

    // Faz bind
    if (bind(listening_socket, (struct sockaddr *) &server, sizeof(server)) < 0){
        perror("Erro ao fazer bind");
        close(listening_socket);
        return -1;
    }

    // Esta chamada diz ao SO que esta é uma socket para receber pedidos
    if (listen(listening_socket, 0) < 0){
        perror("Erro ao executar listen");
        close(listening_socket);
        return -1;
    }

    return listening_socket;
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
 * - Ler os bytes da rede, a partir do client_socket indicado;
 * - De-serializar estes bytes e construir a mensagem com o pedido,
 *   reservando a memória necessária para a estrutura message_t.
 */
struct message_t *network_receive(int client_socket){
    int msg_size;
    char* msg;

    if(read_all(client_socket, &msg_size, sizeof(int)) < 0){
		perror("Erro ao receber tamnaho dos dados do cliente");
		close(client_socket);
    }

    printf("---Expeced size: %d---\n", msg_size);

    msg = malloc(msg_size);

    if(read_all(client_socket, msg, msg_size) < 0){
		perror("Erro ao receber dados do cliente");
		close(client_socket);
    }

    //TODO De-serialise here

    return msg;
}


/* Esta função deve:
 * - Serializar a mensagem de resposta contida em msg;
 * - Libertar a memória ocupada por esta mensagem;
 * - Enviar a mensagem serializada, através do client_socket.
 */
int network_send(int client_socket, struct message_t *msg){
    int msg_size;

    //TODO serialise here

    msg_size = sizeof(char) * (strlen(msg) + 1); //switch this for protobuf get pakage size

    if(write_all(client_socket, &msg_size, sizeof(int)) < 0){
        perror("Erro ao enviar tamanho da resposta ao cliente");
        close(client_socket);
    }

    printf("---Reported size: %d---\n", msg_size);

    if(write_all(client_socket, msg, msg_size) < 0){
        perror("Erro ao enviar resposta ao cliente");
    	close(client_socket);
    }
    
    //free memory here
    free(msg);

    return 0;
}

/* Esta função deve:
 * - Aceitar uma conexão de um cliente;
 * - Receber uma mensagem usando a função network_receive;
 * - Entregar a mensagem de-serializada ao skeleton para ser processada;
 * - Esperar a resposta do skeleton;
 * - Enviar a resposta ao cliente usando a função network_send.
 */
int network_main_loop(int listening_socket){

    //Aceitar uma conexão de um cliente
    //while(1){
        //call network_receive()
        //call invoke()
        //call network_send(return from invoke())
    //}

    int connsockfd, connected;
    struct sockaddr_in client;
    socklen_t size_client = sizeof(client);
    char* msg; //TODO Change this type

    while(1){
        printf("Server listening...\n");
        if ((connsockfd = accept(listening_socket,(struct sockaddr *) &client, &size_client)) < 0) {
            perror("Erro ao aceitar o cliente");
        	close(connsockfd);
            return(-1);
        }

        printf("Connection established in port: %d\n", connsockfd);
        connected = 1;


        while (connected) {

    		msg = (char*)network_receive(connsockfd);

            //-----DEBUG TO BE CHANGED-----

            if(strcmp(msg, "close") == 0){
                // Fecha socket referente a esta conexão
                free(msg);
                msg = malloc(strlen("closing") + 1);
                strcpy(msg, "closing");
                network_send(connsockfd, msg);
    		    close(connsockfd);
                connected = 0;
                printf("Closed connection with port: %d\n\n", connsockfd);
            }
            else{
                printf("Recieved: %s\n", msg);
                msg[0] = toupper(msg[0]);
                printf("Sending: %s\n", msg);

                //-----UNTIL HERE-----

                network_send(connsockfd, msg);
            }
        }
    }
}

/* A função network_server_close() liberta os recursos alocados por
 * network_server_init().
 */
int network_server_close(){
    return 0;
}
