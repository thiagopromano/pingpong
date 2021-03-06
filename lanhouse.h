

#include "structs.h"

#define PORTA 12346      // Porta de conexÃ£o na qual os clientes se conectarÃ£o
#define FilaConexao 10   // NÃºmero de conexÃµes em fila
#define MAXDATASIZE 2000 // Tamanho mÃ¡ximo da recepÃ§Ã£o

char enderecoServidor[25] = "";

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
        perror("Erro ao iniciar EndPoint\n");
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
        perror("Erro ao iniciar binding\n");
        return;
    }

    estado = 0;
    // Iniciar processo de escuta por clientes
    estado = listen(SocketServidor, // Descritor do Socket
                    FilaConexao);   // Backlog: nÃºmero de conexÃµes a serem enfileiradas

    switch (estado)
    {
    case -1:
        perror("Houve um erro ao iniciar processo de escuta\n");
        return;
    case EADDRINUSE:
        perror("Outro socket estÃ¡ escutando na mesma porta\n");
        return;
    default:
        break;
    }

    while (1)
    {

        tamanho = sizeof(struct sockaddr_in);
        printf("Esperando a conexao do amiguinho!\n");
        SocketCliente = accept(SocketServidor,                      // Descritor do Socket
                               (struct sockaddr *)&EnderecoCliente, // Estrutura de endereÃ§o
                               &tamanho);                           // Tamanho da estrutura

        if (SocketCliente < 0)
        {
            perror("Erro em aceitar conexao\n");
            continue;
        }
        game->conectado = 1;
        while (1)
        {
            float y;
            //printf("Recebendo Status\n");
            long numbytes = recv(SocketCliente,
                                 &y,
                                 MAXDATASIZE,
                                 0);
            game->p2.posY = y;
            //printf("Enviando Status\n");
            if (send(SocketCliente, game, sizeof(GameState), 0) == -1)
            {
                perror("Faio Enviar");
                return;
            }
            usleep(500);
        }

        close(SocketCliente);
    }
}

void cliente(void *t)
{

    GameState *game = (GameState *)t;
    int ClienteSocket;
    int numbytes;
    char buf[MAXDATASIZE];
    struct hostent *he;
    struct sockaddr_in EnderecoServidor;
    int estado = 0;

    he = gethostbyname(enderecoServidor); // Endere<C3><83><C2><A7>o do servidor
    if (he < 0)
    {
        herror("Erro ao converter Endere<C3><83><C2><A7>o do Servidor");
        exit(1);
    }

    ClienteSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (ClienteSocket == -1)
    {
        perror("Erro ao inicar EndPoint");
        exit(1);
    }

    EnderecoServidor.sin_family = AF_INET;
    EnderecoServidor.sin_port = htons(PORTA);
    EnderecoServidor.sin_addr = *((struct in_addr *)he->h_addr);
    bzero(&(EnderecoServidor.sin_zero), 8);

    estado = connect(ClienteSocket,
                     (struct sockaddr *)&EnderecoServidor,
                     sizeof(struct sockaddr));

    switch (estado)
    {
    case -1:
        perror("Erro ao conectar");
        return;
    case EADDRINUSE:
        perror("O Endere<C3><83><C2><A7>o j<C3><83><C2><A1> est<C3><83><C2><A1> em uso");
        return;
    default:
        break;
    }
    while (1)
    {
        usleep(1000);
        float y = game->p2.posY;
        if (send(ClienteSocket, &y, sizeof(float), 0) == -1)
        {
            perror("Faio Enviar");
            return;
        }
        GameState temp;

        
        numbytes = recv(ClienteSocket,
                        &temp,
                        MAXDATASIZE,
                        0);
        temp.p2.posY = game->p2.posY;
        memcpy(game,&temp,sizeof(GameState));
        
        if (numbytes <= 0)
        {
            perror("Problemas ao receber");
            exit(1);
        }
    }
    close(ClienteSocket);

    return;
}