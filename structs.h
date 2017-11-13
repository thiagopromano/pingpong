
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netdb.h>
#include <netinet/in.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

typedef struct{
    char nome[100];
    int pontos;
    int posX;
}Player;

typedef struct{
    Player p1;
    Player p2;    
    int posX,posY;
    int conectado;
}GameState;

