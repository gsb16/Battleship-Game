#include <stdlib.h>
#include <stdio.h>
#include "jogo.h"

int clean_stdin()
{
    while (getchar()!='\n');
    return 1;
}

t_coord leCoord(){
	t_coord coord;
	int t;
	while(1){
		printf("[jogada] Coordenadas (x y | 1 =< x,y <= 5): ");
		t = scanf("%d %d", &coord.x, &coord.y);
		clean_stdin();
		if(((coord.x <=5) && (coord.x >= 1)) &&
		   ((coord.y <=5) && (coord.y >= 1)) && (t == 2)){
			return coord;
		}
		printf("[erro] Coordenadas inválidas\n");
	}

	if(t) t = 2;
}

int leAtaque (t_coord *coord, jogador *jogadores, int id){
	int alvo = 5, t;

	while(1){
		while((alvo > 3) || (alvo < 0)){
			printf("[jogada] Jogador alvo do ataque (0..3): ");
			t = scanf("%d", &alvo);
			clean_stdin();
		}
		if ((jogadores[alvo].status != 'C') && (alvo != id)){
			printf("[jogada] Entre com a posição (x y | 1 <= x,y <= 5) do ataque\n");
			*coord = leCoord();
			return alvo;
		} else {
			alvo = -1;
		}
		printf("[erro] Alvo inválido\n");
	}

	if(t) t = 2;

}

void montaMensagem(char dados[8], char coordx, char coordy, char hit, char sent, char nome){
	dados[0] = coordx;
	dados[1] = coordy;
	dados[2] = hit;
	dados[3] = sent;
	dados[4] = nome;
}

pnavio *colocaNavios(){
	pnavio *navios = (pnavio *)malloc(sizeof(pnavio) * 6);
	char c;
	int colocado = 0, t;
	t_coord coord;

	while(colocado == 0){
		printf("[jogada] Entre com o centro do navio\n");
		coord = leCoord();
		printf("[jogada] Entre com a orientação do navio\n");
		printf("[jogada] Orientação (V ou H): ");
		t = scanf(" %c", &c);
		if(((coord.x <=5) && (coord.x >= 1)) &&
		   ((coord.y <=5) && (coord.y >= 1)) &&
		   (((c=='H') && (coord.x >= 2) && (coord.x <= 4)) || (c=='V')) &&
		   (((c=='V') && (coord.y >= 2) && (coord.y <= 4)) || (c=='H'))){
			colocado = 1;
			for(int i = -1; i < 2; i++){
				if(c=='V'){
					navios[i+1].coord.x = coord.x;
					navios[i+1].coord.y = coord.y+i;
				} else {
					navios[i+1].coord.x = coord.x+i;
					navios[i+1].coord.y = coord.y;
				}
				navios[i+1].status = 'A';
				navios[i+1].sentido = c;
			}
		} else {
			printf("[erro] Orientação/Posição Invalida\n");
		}
	}

	colocado = 0;
	while(colocado == 0){
		colocado = 1;
		printf("[jogada] Entre com o centro do navio\n");
		coord = leCoord();
		printf("[jogada] Entre com a orientação do navio\n");
		printf("[jogada] Orientação (V ou H): ");
		t = scanf(" %c", &c);
		if(((coord.x <=5) && (coord.x >= 1)) &&
		   ((coord.y <=5) && (coord.y >= 1)) &&
		   (((c=='H') && (coord.x >= 2) && (coord.x <= 4)) || (c=='V')) &&
		   (((c=='V') && (coord.y >= 2) && (coord.y <= 4)) || (c=='H'))){
			colocado = 1;
			for(int k = -1; k< 2; k++){
				for(int i = 0; i < 3; i++){
					if(c == 'V'){
						if((coord.x == navios[i].coord.x) && (coord.y+k == navios[i].coord.y)){
							colocado = 0;
						}
					} else {
						if((coord.x+k == navios[i].coord.x) && (coord.y == navios[i].coord.y)){
							colocado = 0;
						}
					}
				}
			}
			if(colocado == 1){
				for(int i = -1; i < 2; i++){
					if(c=='V'){
						navios[i+4].coord.x = coord.x;
						navios[i+4].coord.y = coord.y+i;
					} else {
						navios[i+4].coord.x = coord.x+i;
						navios[i+4].coord.y = coord.y;
					}
					navios[i+4].status = 'A';
					navios[i+4].sentido = c;
				}
			} else {
				printf("[erro] Orientação/Posição Invalida\n");
			}
		} else {
			colocado = 0;
			printf("[erro] Orientação/Posição Invalida\n");
		}
	}

	if(t) t = 2;
	return navios;
}

int afundou(pnavio *navios){
	int q1 = 1;
	int q2 = 1;
	for(int i = 0; i < 3; i++){
		if(navios[i].status != 'Q'){
			q1 = 0;
		}
	}
	for(int i = 3; i < 6; i++){
		if(navios[i].status != 'Q'){
			q2 = 0;
		}
	}

	if(q1) return 1;
	if(q2) return 2;
	return 0;
}

int levaAtaque(t_coord *coord, pnavio *navios){
	int hit = 0;
	for(int i = 0; i < 6; i++){
		if((navios[i].coord.x == coord->x) && (navios[i].coord.y == coord->y)){
			if(navios[i].status == 'A'){
				navios[i].status = 'Q';
				hit = -1;
			}
		}
	}

	int afundado = afundou(navios);
	int meio = 0;
	if(afundado > 0){
		int i = afundado?0:3;
		int k = i+3;

		for(i = i; i < k ; i++){
			navios[i].status = 'F';
		}
		meio = (afundado==1)?1:4;
		coord->x = navios[meio].coord.x;
		coord->y = navios[meio].coord.y;
	}

	return afundado?afundado:hit;
}

void imprimeAfundou(int navio, int jogador, int x, int y, char o){
	printf("[informativo] O navio %d do jogador %d afundou nas posições: \n", navio, jogador);
	for(int i = -1; i < 2; i++){
		int xx = x + ((o=='H')?i:0);
		int yy = y + ((o=='V')?i:0);
		printf("(%d,%d)\n", xx, yy);
	}
}
