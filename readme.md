# Rede em Anel - Batalha Naval

## Autores:
- Guilherme Bastos de Oliveira
- Gabriel de Souza Barreto

## Objetivo:
Trabalho para a disciplina de Redes 1
- Jogo de batalha naval (4 jogadores) em uma rede em anel

## Makefile:
- *make:* compila o programa
- *make clean:* deleta arquivos temporários de compilação
- *make purge:* executa clean e remove executavel
- *make remake:* executa purge e compila

## Utilização:
```
	./bnaval <ip0> <ip1> <ip2> <ip3> <porta0> <porta1> <porta2> <porta3> <idJogador> <primeiro>
```
### Onde:
- __ipX:__ é o ip do jogador de id X
- __portaX:__ é a porta que o ipX irá escutar
- __idJogador:__ é um número que identifica o jogador (0..3)
- __primeiro:__ número que representa se este é o primeiro jogador desta partida (1 para sim, 0 para não)

## Etapas do jogo:
1. Jogadores posicionam navios no tabuleiro
2. Primeiro jogador escolhe alvo e local do ataque
3. Passa a vez para próximo jogador
4. Próximo jogador escolhe alvo e local do ataque
5. Jogador atual é único jogar ativo?
	- Não: Volta para passo 3
	- Sim: Fim de jogo

## Mensagem (12 bytes, vetor de char)
```
[tipo|dest|orig|coordx|coordy|hit|sentido|jogador|sequência|gameover|vazio|status]
|              |_______________dados[8] e dadosbkp[8]_____________________|      |
|____________________________________dados2[12]__________________________________|
```
### Campos
0. __tipo:__ tipo de mensagem (ataque, bastão ou informação)
1. __dest:__ número do jogador destino da mensagem (0..3)
2. __ori:__ número do jogador origem da mensagem (0..3)
3. __coordx:__ número da coordenada x (valor de x no ataque ou valor de x do centro do navio afundado)
4. __coordy:__ número da coordenada y (valor de y no ataque ou valor de y do centro do navio afundado)
5. __hit:__ campo que avisa se o ataque acertou algum navio ou se o navio afundou
6. __sentido:__ informa o sentido do navio afundado (H=horizontal ou V=vertical)
7. __jogador:__ número do jogador cujo barco foi afundado
8. __sequência:__ contagem da sequência  para timeout (serve para checar se aquela mensagem é igual à recebida anteriormente)
9. __gameover:__ marca o término do jogo
10. __vazio:__ campo não utilizado
11. __status:__ status atual da mensagem ("indo" = em trânsito ou recebida)

## Estruturas de Dados
```
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
```

## Decisões de projeto
- Bastão: O bastão começa com o jogador marcado no último argumento da chamada de execução.
- Uso dos sockets: Cada máquina abre dois sockets, um recebe mensagens e outra envia mensagens.
- Aviso de navio afundado: O jogador que efetuou o ataque que resultou no afundamento de um navio imprime um aviso para si, mantém o bastão e envia uma mensagem informando o evento.
- Navios: O navio 1 é o primeiro navio a ser afundado e o navio 2 é o segundo.
- Status do jogador: Todos os jogadores começam com status 'A'. Quando um navio é afundado o status é incrementado em 1. Caso o status seja C, o jogador está fora do jogo e só permanece no anel repassando as mensagens.
- Jogador eliminado: O jogador apenas recebe e repassa as mensagens enquanto não receber uma mensagem marcada no campo de fim de jogo.
