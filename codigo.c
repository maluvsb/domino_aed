#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

void insertionSort(int arr[], int size);

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
void inserirNaMesa(Fila *fila, PecaDomino peca) {
  Node *novoNode = (Node *)malloc(sizeof(Node));
  novoNode->peca = peca;
  novoNode->proximo = NULL;

  if (filaVazia(fila)) {
    fila->frente = novoNode;
    fila->tras = novoNode;
  } else {
    int ladoEscolhido;
    printf("Escolha o lado para adicionar a peça (1 - Frente, 2 - Trás): ");
    scanf("%d", &ladoEscolhido);
    if (ladoEscolhido == 1) {
      novoNode->proximo = fila->frente;
      fila->frente = novoNode;
    } else if (ladoEscolhido == 2) {
      fila->tras->proximo = novoNode;
      fila->tras = novoNode;
    }
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

void calcularRanking(Fila *filaJogadores, int *ranking) {
    int numJogadores = 0;
    Node *nodeAtual = filaJogadores->frente;

    while (nodeAtual != NULL) {
        numJogadores++;
        nodeAtual = nodeAtual->proximo;
    }

    Node *jogadores[numJogadores]; // Array para armazenar os ponteiros para os jogadores
    nodeAtual = filaJogadores->frente;

    for (int i = 0; i < numJogadores; i++) {
        jogadores[i] = nodeAtual; // Armazena o ponteiro para cada jogador no array
        nodeAtual = nodeAtual->proximo;
    }

    // Ordenação utilizando Insertion Sort
    for (int i = 1; i < numJogadores; i++) {
        Node *jogadorAtual = jogadores[i];
        int pontosAtual = contarPontos(jogadorAtual);
        int numPecasAtual = contarNumPecas(jogadorAtual);
        int j = i - 1;

        while (j >= 0 && (contarPontos(jogadores[j]) < pontosAtual ||
                          (contarPontos(jogadores[j]) == pontosAtual &&
                           contarNumPecas(jogadores[j]) < numPecasAtual))) {
            jogadores[j + 1] = jogadores[j];
            j--;
        }

        jogadores[j + 1] = jogadorAtual;
    }

    // Atualiza o ranking com base na ordem dos jogadores no array ordenado
    for (int i = 0; i < numJogadores; i++) {
        ranking[i + 1]++; // A posição do jogador no array ordenado é a posição no ranking
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
    printf("Escolha inválida. Digite um número válido.\n");
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
    printf("Escolha inválida. Digite um número válido.\n");
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
// printf("\n\nEscolha uma peça:");
// scanf("%d",&escolha);
// PecaDomino teste = removerPeca(&jogador1,escolha);
// imprimirFila(&jogador1);
// enfileirar(&mesa, teste);
// printf("\n\n-----MESA-----\n");
// imprimirFila(&mesa);
// mesa.frente->peca.pLado = teste.pLado;
// mesa.tras->peca.sLado = teste.sLado;
// printf("\nprimeiro lado : %d", mesa.frente->peca.pLado);
// printf("\nsegundo lado: %d", mesa.tras->peca.sLado);

void mesaDomino(Fila *fila, PecaDomino peca, int ordem) {
  Node *novoNode = (Node *)malloc(sizeof(Node));
  novoNode->peca = peca;
  novoNode->proximo = NULL;

  if (filaVazia(fila)) {
    fila->frente = novoNode;
    fila->tras = novoNode;
  } else {
    if (ordem == 1) {
      // Jogador 1 começa, verifique se pLado da peça é igual à frente ou ao
      // lado da frente
      if (peca.pLado == fila->frente->peca.pLado ||
          peca.pLado == fila->frente->peca.sLado) {
        fila->frente->peca.pLado = peca.sLado;
      } else {
        fila->tras->peca.sLado = peca.pLado;
      }
    } else {
      // Jogador 2 começa, verifique se sLado da peça é igual à frente ou ao
      // lado da frente
      if (peca.sLado == fila->frente->peca.pLado ||
          peca.sLado == fila->frente->peca.sLado) {
        fila->frente->peca.pLado = peca.pLado;
      } else {
        fila->tras->peca.sLado = peca.sLado;
      }
    }

    fila->tras->proximo = novoNode;
    fila->tras = novoNode;
  }

  // Verificar se algum jogador ficou sem peças -- faço isso na main
  // if (filaVazia(&jogador1)) {
  //     printf("Jogador 1 ganhou!\n");
  // } else if (filaVazia(&jogador2)) {
  //     printf("Jogador 2 ganhou!\n");
  // }
}

int main() {
  int escolha, peca, cont_jogador1 = 0, cont_jogador2 = 0;
  PecaDomino teste;
  Fila fila;

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
    printf(" -------------Menu------------- \n\n"); // comente o menu inteiro
                                                    // para executar as funções
    printf(" 1. Iniciar Jogo\n");
    printf(" 2. Mostrar o placar\n");
    printf(" 3. Encerrar Jogo\n");
    printf("\nDigite o número da opção desejada: ");
    scanf("%d", &escolha);

    switch (escolha) {
      
    case 1:
      // system("clear");
      // printf("\nCarregando Mão dos Jogadores 1 e 2:\n");
      // printf("\t\t\t[     ]");
      // fflush(stdout);
      // sleep_ms(1000);
      // system("clear");
      // printf("\nCarregando Mão dos Jogadores 1 e 2:\n");
      // printf("\t\t\t[|    ]");
      // fflush(stdout);
      // sleep_ms(1000);
      // system("clear");
      // printf("\nCarregando Mão dos Jogadores 1 e 2:\n");
      // printf("\t\t\t[||   ]");
      // fflush(stdout);
      // sleep_ms(1000);
      // system("clear");
      // printf("\nCarregando Mão dos Jogadores 1 e 2:\n");
      // printf("\t\t\t[|||  ]");
      // fflush(stdout);
      // sleep_ms(1000);
      // system("clear");
      // printf("\nCarregando Mão dos Jogadores 1 e 2:\n");
      // printf("\t\t\t[|||| ]");
      // fflush(stdout);
      // sleep_ms(1000);
      // system("clear");
      // printf("\nCarregando Mão dos Jogadores 1 e 2:\n");
      // printf("\t\t\t[|||||]");
      // fflush(stdout);
      // sleep_ms(1000);
      // system("clear");
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

      printf("\n");
      printf("\n");
      int escolha, randomico;
      srand(time(NULL));
      randomico = (rand() % 2) + 1;
      if (filaVazia(&mesa)) {

        if (randomico == 1) {
          printf("\n+Vez do jogador 1.");
          printf("\nEscolha uma peça: ");
          scanf("%d", &escolha);
          teste = removerPeca(&jogador1, escolha);
          enfileirar(&mesa, teste);
        }
        if (randomico == 2) {
          printf("\n+Vez do jogador 2.");
          printf("\nEscolha uma peça: ");
          scanf("%d", &escolha);
          teste = removerPeca(&jogador2, escolha);
          enfileirar(&mesa, teste);
        }
      }
      if (!filaVazia(&mesa)) {
        if (randomico == 1) {
          while (!filaVazia(&jogador1) && !filaVazia(&jogador2)) {
            printf("\n -Vez do jogador 2: ");
            printf("\nEscolha uma peça: ");
            scanf("%d", &escolha);
            teste = removerPeca(&jogador2, escolha);
            enfileirar(&mesa, teste);
            printf("\n -Vez do jogador 1: ");
            printf("\nEscolha uma peça: ");
            scanf("%d", &escolha);
            teste = removerPeca(&jogador1, escolha);
            enfileirar(&mesa, teste);
            if (filaVazia(&jogador1)) {
              printf("\nJogador1 Ganhou!\n");
              cont_jogador1++;
              while (!filaVazia(&jogador2)) {
                desenfileirar(&jogador2);
              }
              break;
            }
            if (filaVazia(&jogador2)) {
              printf("\nJogador2 Ganhou!\n");
              cont_jogador2++;
              while (!filaVazia(&jogador1)) {
                desenfileirar(&jogador1);
              }
              break;
            }
          }
        } else {
          while (!filaVazia(&jogador1) && !filaVazia(&jogador2)) {
            printf("\n=Vez do jogador 1: ");
            printf("\nEscolha uma peça: ");
            scanf("%d", &escolha);
            teste = removerPeca(&jogador1, escolha);
            enfileirar(&mesa, teste);
            printf("\n=Vez do jogador 2: ");
            printf("\nEscolha uma peça: ");
            scanf("%d", &escolha);
            teste = removerPeca(&jogador2, escolha);
            enfileirar(&mesa, teste);
            if (filaVazia(&jogador1)) {
              printf("\nJogador1 Ganhou!\n");
              cont_jogador1++;
              while (!filaVazia(&jogador2)) {
                desenfileirar(&jogador2);
              }
              break;
            }
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

      printf("\n\n-----MESA-----\n");
      imprimirFila(&mesa);
      // mesa.frente->peca.pLado = teste.pLado;
      // mesa.tras->peca.sLado = teste.sLado;
      // printf("\nprimeiro lado : %d", teste.pLado);
      // printf("\nsegundo lado: %d", mesa.tras->peca.sLado);

      printf(
          "--------------------------------X--------------------------------");

      printf("\n");
      printf("\n");

      // printf("\nSoma dos dois lados das pedras de cada jogador: \n");

      // printf("\nJogador 1: \n");
      // calcularSomaLadosJogador(&jogador1);

      // printf("\n");
      // printf("\n");

      // printf("\nJogador 2: \n");
      // calcularSomaLadosJogador(&jogador2);

      // printf("\n");
      // printf("\n");
      // printf("\n");

      printf(
          "--------------------------------X--------------------------------");
      break;

    case 2:
        printf("Placar:\n");
        printf("Jogador 1: %d vitórias\n", cont_jogador1);
        printf("Jogador 2: %d vitórias\n", cont_jogador2);
      break;

    case 3:
      printf("\nSaindo do Jogo\n");
      return 0;
      break;

    default:
      printf("\nOpção inválida, digite outro número!\n");
      break;
    }

    printf("\n");

  } while (escolha != 3);

  return 0;
}
