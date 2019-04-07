#ifndef SOCKETS__
#define  SOCKETS__

#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

#define T_ATAQUE 'A'
#define T_BASTAO 'B'
#define T_INFO 'I'
#define S_INDO 'I'
#define S_RECEBIDO 'R'

#include "jogo.h"

int recebe(int sock, char buf[12], int timeout);

void envia(int sock, struct sockaddr_in name, char tipo, jogador alvo, int destino, int origem, char dados[8], char status);

#endif
