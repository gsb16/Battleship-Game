#ifndef JOGO__
#define  JOGO__

typedef struct s_coord{
	int x;
	int y;
} t_coord;

typedef struct s_jogador{
	char *IP;
	int porta;
	int id;
	char status;
} jogador;

typedef struct s_pnavio{
	t_coord coord;
	char status;
	char sentido;
} pnavio;

t_coord leCoord();

pnavio *colocaNavios();

int leAtaque (t_coord *coord, jogador *jogadores, int id);

void montaMensagem(char dados[8], char coordx, char coordy, char hit, char sent, char nome);

int afundou(pnavio *navios);

int levaAtaque(t_coord *coord, pnavio *navios);

void imprimeAfundou(int navio, int jogador, int x, int y, char o);

#endif
