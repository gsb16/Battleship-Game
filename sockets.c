#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <stdio.h>

#include "sockets.h"
#include "jogo.h"

int recebe(int sock, char buf[12], int timeout) {
	struct timeval tempo_to;
	if(timeout){
		tempo_to.tv_sec = 1;
		tempo_to.tv_usec = 0;
		if(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tempo_to, sizeof(tempo_to)) < 0) perror("deu ruim:");
	} else {
		tempo_to.tv_sec = 0;
		tempo_to.tv_usec = 0;
		if(setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tempo_to, sizeof(tempo_to)) < 0) perror("deu ruim:");
	}
	if (read(sock, buf, 12) < 12) return 0;
	return 1;
}

void envia(int sock, struct sockaddr_in name, char tipo, jogador alvo, int destino, int origem, char dados[8], char status){
	char msg[12];
	msg[0] = tipo;
	msg[1] = '0' + destino;
	msg[2] = '0' + origem;
	for(int i = 3; i < 11; i++){
		msg[i] = dados[i-3];
	}
	msg[11] = status;

    if (sendto(sock, msg, sizeof(char)*12, 0, (struct sockaddr *) &name, sizeof name) < 0)
    perror("sending datagram message");
}
