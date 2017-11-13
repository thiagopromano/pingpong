#include "structs.h"

#define PORTA 12346      // Porta de conexÃ£o na qual os clientes se conectarÃ£o
#define FilaConexao 10   // NÃºmero de conexÃµes em fila
#define MAXDATASIZE 2000 // Tamanho mÃ¡ximo da recepÃ§Ã£o

void servidor(void *t)
{
    GameState *game = (GameState *)t;

    printf("iniciando como servidor\n");
    int SocketServidor;                  // Socket para escuta do servidor
    int SocketCliente;                   // Socket para comunicaÃ§Ã£o com novo cliente
    struct sockaddr_in EnderecoServidor; // InformaÃ§Ãµes sobre endereÃ§o do servidor
    struct sockaddr_in EnderecoCliente;  // InformaÃ§Ã£o sobre endereÃ§o do cliente
    int tamanho;
    int estado = 0;

    // Cria um ponto de comunicaÃ§Ã£o
    SocketServidor = socket(AF_INET,     // DomÃ­nio: AF_INET - Protocolo IPv4
                            SOCK_STREAM, // Tipo: SOCK_STREAM - via de mÃ£o dupla, confiÃ¡vel, sequencial e baseada em conexÃ£o
                            0);          // Protocolo: Especificar protocolos nÃ£o descritos

    if (SocketServidor == -1)
    {
        perror("Erro ao iniciar EndPoint");
        return;
    }

    EnderecoServidor.sin_family = AF_INET;         // DomÃ­nio: FamÃ­lia de endereÃ§os IPv4
    EnderecoServidor.sin_port = htons(PORTA);      // Porta: Porta de comunicaÃ§Ã£o
    EnderecoServidor.sin_addr.s_addr = INADDR_ANY; // Inclui o endereÃ§o IP da comunicaÃ§Ã£o de entrada
    bzero(&(EnderecoServidor.sin_zero), 8);        // Limpa a estrutura

    // Vincula novo socket
    estado = bind(SocketServidor,                       // Descritor do Socket
                  (struct sockaddr *)&EnderecoServidor, // EndereÃ§o em formato sockaddr
                  sizeof(struct sockaddr));             // Tamanho da estrutura de endereÃ§o

    if (estado == -1)
    {
        perror("Erro ao iniciar binding");
        return;
    }

    estado = 0;
    // Iniciar processo de escuta por clientes
    estado = listen(SocketServidor, // Descritor do Socket
                    FilaConexao);   // Backlog: nÃºmero de conexÃµes a serem enfileiradas

    switch (estado)
    {
    case -1:
        perror("Houve um erro ao iniciar processo de escuta");
        return;
    case EADDRINUSE:
        perror("Outro socket estÃ¡ escutando na mesma porta");
        return;
    default:
        break;
    }

    while (1)
    {

        tamanho = sizeof(struct sockaddr_in);
        printf("Esperando a conexao do amiguinho!")
        SocketCliente = accept(SocketServidor,                      // Descritor do Socket
                               (struct sockaddr *)&EnderecoCliente, // Estrutura de endereÃ§o
                               &tamanho);                           // Tamanho da estrutura

        if (SocketCliente < 0)
        {
            perror("Erro em aceitar conexÃ£o");
            continue;
        }

        while (1)
        {
            printf("Enviando Status\n");
            if (send(SocketCliente, game, sizeof(GameState), 0) == -1)
            {
                perror("Faio Enviar");
                return;
            }
            int x;
            printf("Recebendo Status\n")
            long numbytes = recv(SocketCliente,
                                 &x,
                                 MAXDATASIZE,
                                 0);
            game->p2.posX = x;
            usleep(500);
        }

        close(SocketCliente);
    }
}