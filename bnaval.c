#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <unistd.h>
#include <netdb.h>

#include "jogo.h"
#include "sockets.h"

int main ( int argc, char *argv[]){
	int flagbastao = atoi(argv[10]), id = atoi(argv[9]), prox = (id+1) % 4, alvo = 0, aux, seq = 0;
	char dados[8], dadosbkp[8], dados2[12];
	t_coord coord;
	dados[6] = 0;
	int socke, sockr, length;
	struct sockaddr_in namee, namer;
	jogador jogadores[4];

	for(int i = 1; i <= 4; i++){
		jogadores[i-1].IP = argv[i];
		jogadores[i-1].status = 'A';
		jogadores[i-1].id = i;
	}
	for(int i = 5; i <= 8; i++){
		jogadores[i-5].porta = atoi(argv[i]);
	}

	pnavio *navios = colocaNavios();


	socke = socket(AF_INET, SOCK_DGRAM, 0);
	if (socke < 0){
		perror("opening datagram socket");
		exit(1);
	}

	namee.sin_family = AF_INET;
	namee.sin_addr.s_addr = INADDR_ANY;
	namee.sin_port = htons(jogadores[id].porta);
	if (bind(socke, (struct sockaddr *)&namee, sizeof namee ) < 0){
		perror("binding datagram socket");
		exit(1);
	}

	length = sizeof(namee);
	if (getsockname(socke, (struct sockaddr *)&namee, (void *)&length) < 0){
		perror("getting socket name");
	}

	struct hostent *hp, *gethostbyname();

    sockr = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockr < 0){
        perror("opening datagram socket");
        exit(1);
    }

	hp = gethostbyname(jogadores[prox].IP);
	if(hp == 0){
	    fprintf(stderr, "%s: Unknown host\n", jogadores[prox].IP);
        exit(2);
    }
    bcopy((char *)hp -> h_addr, (char *) &namer.sin_addr, hp->h_length);
	namer.sin_family = AF_INET;
	namer.sin_port = htons(jogadores[prox].porta);

	while(jogadores[id].status != 'C'){
		if (flagbastao){
			alvo = leAtaque(&coord, jogadores, id);
			montaMensagem(dados, coord.x, coord.y, 0, 0, 0);
			seq++;
			dados[5] = seq;
			envia(sockr, namer, T_ATAQUE, jogadores[prox], alvo, id, dados, S_INDO);
			while(!recebe(socke, dados2, 1)){
				envia(sockr, namer, T_ATAQUE, jogadores[prox], alvo, id, dados, S_INDO);
			}
		} else {
			recebe(socke, dados2, 0);
		}

		if(dados2[0] == T_BASTAO){
			flagbastao = 1;
			printf("[turno] Sua Vez\n");
		} else if ((dados2[2]-'0' == id)){
			if (dados2[5] == 'F'){
				for(int i = 0; i < 8; i++){
					dados[i] = dados2[i+3];
				}
				seq++;
				dados[5] = seq;
				int auxxx = 0;

				jogadores[(int)dados2[7]].status++;
				for(int i = 0; i < 4; i++){
					if(jogadores[i].status != 'C'){
						auxxx++;
					}
				}
				if(auxxx==1){
					dados[6] = 1;
					printf("[fim de jogo] Você ganhou!\n");
					envia(sockr, namer, T_INFO, jogadores[prox], (id-1+4) % 4, id , dados, S_INDO);
					exit(0);
				}

				envia(sockr, namer, T_INFO, jogadores[prox], (id-1+4) % 4, id , dados, S_INDO);
				while(!recebe(socke, dados2, 1)){
					envia(sockr, namer, T_INFO, jogadores[prox], (id-1+4) % 4, id , dados, S_INDO);
				}

				for(int i = 0; i < 8; i++){
					dados[i] = dados2[i+3];
				}

				imprimeAfundou(jogadores[(int)dados2[7]].status-'A', (int)dados2[7], (int)dados2[3], (int)dados2[4], dados2[6]);
			} else if (dados2[5] == 'H'){
				printf("[resultado] Acertou!\n");
			} else {
				printf("[resultado] Água\n" );
			}

			flagbastao = -1;
			printf("[turno] Aguarde sua vez\n" );
		} else {
			for(int i = 0; i < 8; i++){
				dados[i] = dados2[i+3];
			}
		}
		if (flagbastao != 1){
			if(flagbastao == 0){
				if ((dados2[1] - '0' == id) && (dados2[0] != T_INFO)){
					coord.x = dados[0], coord.y = dados[1];
					if(seq != dados2[8]){
						seq = dados2[8];
						aux = levaAtaque(&coord, navios);
						if (aux > 0){
							montaMensagem(dados, coord.x, coord.y, 'F', navios[2*aux].sentido , id);
						} else if (aux == -1){
							dados[2] = 'H';
						}
						for(int i = 0; i < 8; i++){
							dadosbkp[i] = dados[i];
						}
					} else {
						for(int i = 0; i < 8; i++){
							dados[i] = dadosbkp[i];
						}
					}
					dados2[11] = S_RECEBIDO;
				} else if(dados2[1] - '0' != id){
					seq = dados2[8];
					dados[5] = seq;
				}
				if (dados2[0] == T_INFO){
					seq = dados2[8];
					jogadores[(int)dados2[7]].status++;
					imprimeAfundou(jogadores[(int)dados2[7]].status-'A', (int)dados2[7], dados2[3], dados2[4], dados2[6]);

					if (dados2[1] - '0' == id){
						dados2[11] = S_RECEBIDO;
					}
				}
				dados[5] = seq;
				envia(sockr, namer, dados2[0], jogadores[prox], dados2[1] - '0', dados2[2] - '0', dados, dados2[11]);
			} else {
				seq = dados2[8];
				dados[5] = seq;
				flagbastao = 0;
				envia(sockr, namer, T_BASTAO, jogadores[prox], prox, id, dados, S_INDO);
			}
		}
	}

	while(!dados2[9]){
		recebe(socke, dados2, 0);
		for(int i = 0; i < 8; i++){
			dados[i] = dados2[i+3];
		}
		envia(sockr, namer, dados2[0], jogadores[prox], dados2[1] - '0', dados2[2] - '0', dados, dados2[11]);
	}

	printf("[fim de jogo] Você perdeu\n");

	return 0;
}
