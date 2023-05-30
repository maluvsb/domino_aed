#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>

typedef struct {
  int pLado;
  int sLado;
} PecaDomino;

typedef struct Node {
  PecaDomino peca;
  struct Node *proximo;
} Node;

typedef struct {
  Node *frente;
  Node *tras;
} Fila;

typedef struct Player{
  char name[50];
  int victories;
} Player;

void inicializarFila(Fila *fila) {
  fila->frente = NULL;
  fila->tras = NULL;
}

int filaVazia(Fila *fila) { return (fila->frente == NULL); }

void enfileirar(Fila *fila, PecaDomino peca) {
  Node *novoNode = (Node *)malloc(sizeof(Node));
  novoNode->peca = peca;
  novoNode->proximo = NULL;

  if (filaVazia(fila)) {
    fila->frente = novoNode;
    fila->tras = novoNode;
  } else {
    fila->tras->proximo = novoNode;
    fila->tras = novoNode;
  }
}

void desenfileirar(Fila *fila) {
  if (filaVazia(fila)) {
    printf("A fila está vazia.\n");
    return;
  }

  Node *nodeRemovido = fila->frente;
  fila->frente = fila->frente->proximo;
  free(nodeRemovido);

  if (fila->frente == NULL) {
    fila->tras = NULL;
  }
}

void imprimirFila(Fila *fila) {
  if (filaVazia(fila)) {
    printf("A fila está vazia.\n");
    return;
  }

  Node *nodeAtual = fila->frente;
  int contador = 1;

  while (nodeAtual != NULL) {
    printf("Peça %d: %d|%d\n", contador, nodeAtual->peca.pLado,
           nodeAtual->peca.sLado);
    nodeAtual = nodeAtual->proximo;
    contador++;
  }
}

void embaralharFila(Fila *fila) {
  if (filaVazia(fila)) {
    printf("A fila está vazia.\n");
    return;
  }

  int tamanhoFila = 0;
  Node *nodeAtual = fila->frente;

  while (nodeAtual != NULL) {
    tamanhoFila++;
    nodeAtual = nodeAtual->proximo;
  }

  int *numerosAleatorios = (int *)malloc(tamanhoFila * sizeof(int));
  for (int i = 0; i < tamanhoFila; i++) {
    numerosAleatorios[i] = i;
  }

  srand(time(NULL)); // aleatoriedade das pedras

  for (int i = tamanhoFila - 1; i > 0; i--) {
    int j = rand() % (i + 1);
    int temp = numerosAleatorios[i];
    numerosAleatorios[i] = numerosAleatorios[j];
    numerosAleatorios[j] = temp;
  }

  Fila filaEmbaralhada;
  inicializarFila(&filaEmbaralhada);

  nodeAtual = fila->frente;
  for (int i = 0; i < tamanhoFila; i++) {
    for (int j = 0; j < numerosAleatorios[i]; j++) {
      nodeAtual = nodeAtual->proximo;
    }
    enfileirar(&filaEmbaralhada, nodeAtual->peca);
    nodeAtual = fila->frente;
  }

  *fila = filaEmbaralhada;

  free(numerosAleatorios);
}

void distribuirPecas(Fila *fila, Fila *jogador1, Fila *jogador2) {
  for (int i = 0; i < 6; i++) {
    if (!filaVazia(fila)) {
      enfileirar(jogador1, fila->frente->peca);
      desenfileirar(fila);
    }
    if (!filaVazia(fila)) {
      enfileirar(jogador2, fila->frente->peca);
      desenfileirar(fila);
    }
  }
}

int somaLadosPeca(PecaDomino peca) { return peca.pLado + peca.sLado; }

void calcularSomaLadosJogador(Fila *jogador) {
  int count = 1;
  if (filaVazia(jogador)) {
    return;
  }

  Node *nodeAtual = jogador->frente;

  while (nodeAtual != NULL) {
    int somaLados = somaLadosPeca(nodeAtual->peca);
    printf("Peça %d: %d | ", count, somaLados);
    nodeAtual = nodeAtual->proximo;
    count++;
  }
}

int contarPontos(Fila *fila) {
  int pontos = 0;
  Node *nodeAtual = fila->frente;
  while (nodeAtual != NULL) {
    pontos += nodeAtual->peca.pLado + nodeAtual->peca.sLado;
    nodeAtual = nodeAtual->proximo;
  }
  return pontos;
}

int contarNumPecas(Node *jogador) {
  int count = 0;
  Node *nodeAtual = jogador;

  while (nodeAtual != NULL) {
    count++;
    nodeAtual = nodeAtual->proximo;
  }

  return count;
}

void insertion_sort_ranking(Player players[], int n) {
  int i, j;
  Player atual;

  for (i = 1; i < n; i++) {
    atual = players[i];
    j = i - 1;

    while (j >= 0 && players[j].victories < atual.victories) {
      players[j + 1] = players[j];
      j--;
    }

    players[j + 1] = atual;
  }
}

void show_ranking(Player players[], int n) {
  insertion_sort_ranking(players, n);

  for (int i = 0; i < n; i++) {
    printf("#%d: %s - Vitórias: %d\n", i + 1, players[i].name,
           players[i].victories);
  }
}

int tamanhoFila(Fila *fila) {
  int tamanho = 0;
  Node *nodeAtual = fila->frente;
  while (nodeAtual != NULL) {
    tamanho++;
    nodeAtual = nodeAtual->proximo;
  }
  return tamanho;
}

void sleep_ms(unsigned int milliseconds) { usleep(milliseconds * 1000); }

PecaDomino removerPeca(Fila *jogador, int escolha) {
  if (escolha < 1 || escolha > tamanhoFila(jogador)) {
    PecaDomino pecaInvalida = {-1, -1}; // Retorna uma peça inválida
    return pecaInvalida;
  }

  Node *nodeAnterior = NULL;
  Node *nodeAtual = jogador->frente;
  int contador = 1;

  while (nodeAtual != NULL && contador != escolha) {
    nodeAnterior = nodeAtual;
    nodeAtual = nodeAtual->proximo;
    contador++;
  }

  if (nodeAtual == NULL) {
    PecaDomino pecaInvalida = {-1, -1}; // Retorna uma peça inválida
    return pecaInvalida;
  }

  PecaDomino pecaRemovida = nodeAtual->peca;

  if (nodeAnterior == NULL) {
    jogador->frente = jogador->frente->proximo;
  } else {
    nodeAnterior->proximo = nodeAtual->proximo;
  }

  if (nodeAtual == jogador->tras) {
    jogador->tras = nodeAnterior;
  }

  free(nodeAtual);

  return pecaRemovida;
}

PecaDomino obterPeca(Fila *jogador, int escolha) {
  if (escolha < 1 || escolha > tamanhoFila(jogador)) {
    PecaDomino pecaInvalida = {-1, -1}; // Retorna uma peça inválida
    return pecaInvalida;
  }

  Node *nodeAtual = jogador->frente;
  int contador = 1;

  while (nodeAtual != NULL && contador != escolha) {
    nodeAtual = nodeAtual->proximo;
    contador++;
  }

  if (nodeAtual == NULL) {
    PecaDomino pecaInvalida = {-1, -1}; // Retorna uma peça inválida
    return pecaInvalida;
  }

  PecaDomino pecaEscolhida = nodeAtual->peca;

  return pecaEscolhida;
}

int main() {
  int escolha, peca, cont_jogador1 = 0, cont_jogador2 = 0;
  int referencia[2];
  PecaDomino teste;
  Fila fila;
  Player players[2];
  do {
    inicializarFila(&fila);

    int count = 0;
    for (int i = 0; i <= 6; i++) {
      for (int j = i; j <= 6; j++) {
        PecaDomino peca;
        peca.pLado = i;
        peca.sLado = j;
        enfileirar(&fila, peca);
        count++;
      }
    }

    embaralharFila(&fila);

    Fila jogador1;
    inicializarFila(&jogador1);

    Fila jogador2;
    inicializarFila(&jogador2);

    Fila mesa;
    inicializarFila(&mesa);
    system("clear");
    printf(" -------------Menu------------- \n\n"); // comente o menu inteiro
                                                    // para executar as funções
    printf(" 1. Iniciar Jogo\n");
    printf(" 2. Mostrar o placar\n");
    printf(" 3. Encerrar Jogo\n");
    printf("\nDigite o número da opção desejada: ");
    scanf("%d", &escolha);

    switch (escolha) {

    case 1:
      system("clear");
      printf("\nCarregando Mão dos Jogadores 1 e 2.\n");
      printf("\t\t\t[     ]");
      fflush(stdout);
      sleep_ms(1000);
      system("clear");
      printf("\nCarregando Mão dos Jogadores 1 e 2..\n");
      printf("\t\t\t[|    ]");
      fflush(stdout);
      sleep_ms(1000);
      system("clear");
      printf("\nCarregando Mão dos Jogadores 1 e 2...\n");
      printf("\t\t\t[||   ]");
      fflush(stdout);
      sleep_ms(1000);
      system("clear");
      printf("\nCarregando Mão dos Jogadores 1 e 2\n");
      printf("\t\t\t[|||  ]");
      fflush(stdout);
      sleep_ms(1000);
      system("clear");
      printf("\nCarregando Mão dos Jogadores 1 e 2.\n");
      printf("\t\t\t[|||| ]");
      fflush(stdout);
      sleep_ms(1000);
      system("clear");
      printf("\nCarregando Mão dos Jogadores 1 e 2..\n");
      printf("\t\t\t[|||||]");
      fflush(stdout);
      sleep_ms(1000);
      system("clear");
      if (!filaVazia(&mesa)) {
        while (!filaVazia(&mesa)) {
          desenfileirar(&mesa);
        }
      }
      distribuirPecas(&fila, &jogador1, &jogador2);
      printf("\nJogador 1:\n");
      imprimirFila(&jogador1);

      printf("\nJogador 2:\n");
      imprimirFila(&jogador2);
      sleep_ms(3000);
      printf("\n");
      printf("\n");
      int escolha, randomico;
      srand(time(NULL));
      randomico = (rand() % 2) + 1;
      if (filaVazia(&mesa)) {
        // primeira jogada
        if (randomico == 1) {
          system("clear");
          printf("\nJogador 1:\n");
          imprimirFila(&jogador1);
          printf("\nVez do jogador 1.");
          printf("\nEscolha uma peça: ");
          scanf("%d", &escolha);
          teste = removerPeca(&jogador1, escolha);
          referencia[0] = teste.pLado;
          referencia[1] = teste.sLado;
          enfileirar(&mesa, teste);
        }
        if (randomico == 2) {
          system("clear");
          printf("\nJogador 2:\n");
          imprimirFila(&jogador2);
          printf("\nVez do jogador 2.");
          printf("\nEscolha uma peça: ");
          scanf("%d", &escolha);
          teste = removerPeca(&jogador2, escolha);
          referencia[0] = teste.pLado;
          referencia[1] = teste.sLado;
          enfileirar(&mesa, teste);
        }
      }
      // fim primeira jogada
      // começo resto das jogadas começando pelo jogador 2
      if (!filaVazia(&mesa)) {
        if (randomico == 1) {
          while (!filaVazia(&jogador1) && !filaVazia(&jogador2)) {
            int cont_gato_lebre_p1 = 7, cont_gato_lebre_p2 = 7;
            system("clear");
            printf("\nJogador 2:\n");
            imprimirFila(&jogador2);
            printf("\nNúmeros disponíveis em cada lado: \n");
            printf("\n\t\t\t\t%d|%d\n", referencia[0], referencia[1]);
            printf("\nVez do jogador 2: ");
            printf("\nEscolha uma peça: ");
            scanf("%d", &escolha);
            if (escolha == 0) {
              break;
            }
            if (escolha > 6 || escolha < 0) {
              system("clear");
              printf("\nJogador 2 tocou!");
              printf("\n ");
              sleep_ms(1000);
            }
            teste = obterPeca(&jogador2, escolha);
            if (teste.pLado == referencia[0]) {
              removerPeca(&jogador2, escolha);
              referencia[0] = teste.sLado;
              cont_gato_lebre_p2--;
            } else if (teste.sLado == referencia[0]) {
              removerPeca(&jogador2, escolha);
              referencia[0] = teste.pLado;
              cont_gato_lebre_p2--;
            } else if (teste.pLado == referencia[1]) {
              removerPeca(&jogador2, escolha);
              referencia[1] = teste.sLado;
              cont_gato_lebre_p2--;
            } else if (teste.sLado == referencia[1]) {
              removerPeca(&jogador2, escolha);
              referencia[1] = teste.pLado;
              cont_gato_lebre_p2--;
            } else if (escolha < cont_gato_lebre_p2 && escolha > 0 &&
                       teste.pLado != referencia[0] &&
                       teste.sLado != referencia[0] &&
                       teste.pLado != referencia[1] &&
                       teste.sLado != referencia[1]) {
              system("clear");
              printf("\nOlha o gato com lebre aí, jogador! Perdeu a vez.");
              printf("\n ");
              sleep_ms(1000);
            }
            enfileirar(&mesa, teste);
            if (filaVazia(&jogador2)) {
              printf("\nJogador2 Ganhou!\n");
              cont_jogador2++;
              while (!filaVazia(&jogador1)) {
                desenfileirar(&jogador1);
              }
              break;
            }
            system("clear");
            printf("\nJogador 1:\n");
            imprimirFila(&jogador1);
            printf("\nNúmeros disponíveis em cada lado: \n");
            printf("\n\t\t\t\t%d|%d\n", referencia[0], referencia[1]);
            printf("\nVez do jogador 1: ");
            printf("\nEscolha uma peça: ");
            scanf("%d", &escolha);
            if (escolha == 0) {
              break;
            }
            if (escolha > 6 || escolha < 0) {
              system("clear");
              printf("\nJogador 1 tocou!");
              printf("\n ");
              sleep_ms(1000);
            }
            teste = obterPeca(&jogador1, escolha);
            if (teste.pLado == referencia[0]) {
              removerPeca(&jogador1, escolha);
              referencia[0] = teste.sLado;
              cont_gato_lebre_p1--;
            } else if (teste.sLado == referencia[0]) {
              removerPeca(&jogador1, escolha);
              referencia[0] = teste.pLado;
              cont_gato_lebre_p1--;
            } else if (teste.pLado == referencia[1]) {
              removerPeca(&jogador1, escolha);
              referencia[1] = teste.sLado;
              cont_gato_lebre_p1--;
            } else if (teste.sLado == referencia[1]) {
              removerPeca(&jogador1, escolha);
              referencia[1] = teste.pLado;
              cont_gato_lebre_p1--;
            } else if (escolha < cont_gato_lebre_p1 && escolha > 0 &&
                       teste.pLado != referencia[0] &&
                       teste.sLado != referencia[0] &&
                       teste.pLado != referencia[1] &&
                       teste.sLado != referencia[1]) {
              system("clear");
              printf("\nOlha o gato com lebre aí, jogador! Perdeu a vez.");
              printf("\n ");
              sleep_ms(1000);
            }
            enfileirar(&mesa, teste);
            // fim do resto das jogadas começando pelo jogador 2
            // começo print dos ganhadores
            if (filaVazia(&jogador1)) {
              printf("\nJogador1 Ganhou!\n");
              cont_jogador1++;
              while (!filaVazia(&jogador2)) {
                desenfileirar(&jogador2);
              }
              break;
            }
            // fim print dos ganhadores
          }
          // começo do resto das jogadas começando pelo jogador 1
        } else {
          while (!filaVazia(&jogador1) && !filaVazia(&jogador2)) {
            int cont_gato_lebre_p1 = 7, cont_gato_lebre_p2 = 7;
            system("clear");
            printf("\nJogador 1:\n");
            imprimirFila(&jogador1);
            printf("\nNúmeros disponíveis em cada lado: \n");
            printf("\n\t\t\t\t%d|%d\n", referencia[0], referencia[1]);
            printf("\nVez do jogador 1: ");
            printf("\nEscolha uma peça: ");
            scanf("%d", &escolha);
            if (escolha == 0) {
              printf("\nJogo interrompido!\n");
              sleep_ms(1000);
              system("clear");
              break;
            }
            if (escolha > 6 || escolha < 0) {
              system("clear");
              printf("\nJogador 1 tocou!");
              printf("\n ");
              sleep_ms(1000);
            }
            teste = obterPeca(&jogador1, escolha);
            if (teste.pLado == referencia[0]) {
              removerPeca(&jogador1, escolha);
              referencia[0] = teste.sLado;
              cont_gato_lebre_p1--;
            } else if (teste.sLado == referencia[0]) {
              removerPeca(&jogador1, escolha);
              referencia[0] = teste.pLado;
              cont_gato_lebre_p1--;
            } else if (teste.pLado == referencia[1]) {
              removerPeca(&jogador1, escolha);
              referencia[1] = teste.sLado;
              cont_gato_lebre_p1--;
            } else if (teste.sLado == referencia[1]) {
              removerPeca(&jogador1, escolha);
              referencia[1] = teste.pLado;
              cont_gato_lebre_p1--;
            } else if (escolha < cont_gato_lebre_p1 && escolha > 0 &&
                       teste.pLado != referencia[0] &&
                       teste.sLado != referencia[0] &&
                       teste.pLado != referencia[1] &&
                       teste.sLado != referencia[1]) {
              system("clear");
              printf("\nOlha o gato com lebre aí, jogador!");
              printf("\n ");
              sleep_ms(1000);
            }
            enfileirar(&mesa, teste);
            if (filaVazia(&jogador1)) {
              printf("\nJogador1 Ganhou!\n");
              cont_jogador1++;
              while (!filaVazia(&jogador2)) {
                desenfileirar(&jogador2);
              }
              break;
            }
            system("clear");
            printf("\nJogador 2:\n");
            imprimirFila(&jogador2);
            printf("\nNúmeros disponíveis em cada lado: \n");
            printf("\n\t\t\t\t%d|%d\n", referencia[0], referencia[1]);
            printf("\nVez do jogador 2: ");
            printf("\nEscolha uma peça: ");
            scanf("%d", &escolha);
            if (escolha == 0) {
              break;
            }
            if (escolha > 6 || escolha < 0) {
              system("clear");
              printf("\nJogador 2 tocou!");
              printf("\n ");
              sleep_ms(1000);
            }
            teste = obterPeca(&jogador2, escolha);
            if (teste.pLado == referencia[0]) {
              removerPeca(&jogador2, escolha);
              referencia[0] = teste.sLado;
              cont_gato_lebre_p2--;
            } else if (teste.sLado == referencia[0]) {
              removerPeca(&jogador2, escolha);
              referencia[0] = teste.pLado;
              cont_gato_lebre_p2--;
            } else if (teste.pLado == referencia[1]) {
              removerPeca(&jogador2, escolha);
              referencia[1] = teste.sLado;
              cont_gato_lebre_p2--;
            } else if (teste.sLado == referencia[1]) {
              removerPeca(&jogador2, escolha);
              referencia[1] = teste.pLado;
              cont_gato_lebre_p2--;
            } else if (escolha < cont_gato_lebre_p2 && escolha > 0 &&
                       teste.pLado != referencia[0] &&
                       teste.sLado != referencia[0] &&
                       teste.pLado != referencia[1] &&
                       teste.sLado != referencia[1]) {
              system("clear");
              printf("\nOlha o gato com lebre aí, jogador!");
              printf("\n ");
              sleep_ms(1000);
            }
            enfileirar(&mesa, teste);
            // fim do resto das jogadas começando pelo jogador 1
            if (filaVazia(&jogador2)) {
              printf("\nJogador2 Ganhou!\n");
              cont_jogador2++;
              while (!filaVazia(&jogador1)) {
                desenfileirar(&jogador1);
              }
              break;
            }
          }
        }
      }

      printf("\n\n-----HISTÓRICO DAS JOGADAS-----\n");
      imprimirFila(&mesa);

      printf("\n");
      printf("\n");

      sleep_ms(5000);
      break;

    case 2:
      players[0].victories=cont_jogador1;
      strcpy(players[0].name,"Jogador 1");
      players[1].victories=cont_jogador2;
      strcpy(players[1].name,"Jogador 2");
      show_ranking(players, 2);
      int sair;
      printf("\nInsira 1 para sair:");
      scanf("%d", &sair);
      if (sair == 1) {
        break;
      } else {
        while (sair != 1) {
          printf("\nDigita o número correto, parceiro(a):");
          scanf("%d", &sair);
        }
        break;
      }

    case 3:
      printf("\n Saindo do Jogo... \n");
      return 0;
      break;

    default:
      printf("\nOpção inválida, digite outro número!\n");
      sleep_ms(2000);
      break;
    }

    printf("\n");

  } while (escolha != 3);

  return 0;
}
