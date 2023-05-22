#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int ladoA;
    int ladoB;
} PecaDomino;

typedef struct {
    PecaDomino peca;
    struct Node* proximo;
} Node;

typedef struct {
    Node* frente;
    Node* tras;
} Fila;

void inicializarFila(Fila* fila) {
    fila->frente = NULL;
    fila->tras = NULL;
}

int filaVazia(Fila* fila) {
    return (fila->frente == NULL);
}

void enfileirar(Fila* fila, PecaDomino peca) {
    Node* novoNode = (Node*)malloc(sizeof(Node));
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

void desenfileirar(Fila* fila) {
    if (filaVazia(fila)) {
        printf("A fila está vazia.\n");
        return;
    }

    Node* nodeRemovido = fila->frente;
    fila->frente = fila->frente->proximo;
    free(nodeRemovido);

    if (fila->frente == NULL) {
        fila->tras = NULL;
    }
}

void imprimirFila(Fila* fila) {
    if (filaVazia(fila)) {
        printf("A fila está vazia.\n");
        return;
    }

    Node* nodeAtual = fila->frente;
    int contador = 1;

    while (nodeAtual != NULL) {
        printf("Peça %d: %d|%d\n", contador, nodeAtual->peca.ladoA, nodeAtual->peca.ladoB);
        nodeAtual = nodeAtual->proximo;
        contador++;
    }
}

void embaralharFila(Fila* fila) {
    if (filaVazia(fila)) {
        printf("A fila está vazia.\n");
        return;
    }

    int tamanhoFila = 0;
    Node* nodeAtual = fila->frente;

    while (nodeAtual != NULL) {
        tamanhoFila++;
        nodeAtual = nodeAtual->proximo;
    }

    int* numerosAleatorios = (int*)malloc(tamanhoFila * sizeof(int));
    for (int i = 0; i < tamanhoFila; i++) {
        numerosAleatorios[i] = i;
    }

    srand(time(NULL));

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

void distribuirPecas(Fila* fila, Fila* jogador1, Fila* jogador2) {
    for (int i = 0; i < 3; i++) {
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

/*void insertionSort(Fila* jogador) {
    if (filaVazia(jogador)) {
        return;
    }

    Node* atual = jogador->frente->proximo;

    while (atual != NULL) {
        PecaDomino peca = atual->peca;
        Node* posicao = atual;
        Node* anterior = atual->proximo;

        while (posicao != jogador->frente && posicao->peca.ladoA < anterior->peca.ladoA) {
            posicao->peca = anterior->peca;
            posicao = anterior;
            anterior = anterior->proximo;
        }

        posicao->peca = peca;
        atual = atual->proximo;
    }
}

  void verificarJogada(Fila* jogador1, Fila* jogador2, Fila* mesa) {
    Node* nodeAtualJogador1 = jogador1->frente;
    Node* nodeAtualJogador2 = jogador2->frente;
    PecaDomino pecaJogada;

/*    while (nodeAtualJogador1 != NULL) {
        if (nodeAtualJogador1->peca.ladoA == nodeAtualJogador1->peca.ladoB) {
            while (nodeAtualJogador2 != NULL) {
                if (nodeAtualJogador2->peca.ladoA == nodeAtualJogador2->peca.ladoB) {
                    if (nodeAtualJogador1->peca.ladoA == nodeAtualJogador2->peca.ladoA) {
                        // Joga o jogador com a peça igual de maior valor
                        if (nodeAtualJogador1->peca.ladoA > nodeAtualJogador2->peca.ladoA) {
                            pecaJogada = nodeAtualJogador1->peca;
                            desenfileirar(jogador1);
                        } else {
                            pecaJogada = nodeAtualJogador2->peca;
                            desenfileirar(jogador2);
                        }
                        enfileirar(mesa, pecaJogada);
                        return;
                    }
                }
                nodeAtualJogador2 = nodeAtualJogador2->proximo;
            }
        }
        nodeAtualJogador1 = nodeAtualJogador1->proximo;
        nodeAtualJogador2 = jogador2->frente;
    } 
  } */

int main() {
    Fila fila;
    inicializarFila(&fila);

    int contador = 0;
    for (int i = 0; i <= 6; i++) {
        for (int j = i; j <= 6; j++) {
            PecaDomino peca;
            peca.ladoA = i;
            peca.ladoB = j;
            enfileirar(&fila, peca);
            contador++;
        }
    }

    printf("Fila original:\n");
    imprimirFila(&fila);

    printf("\nFila embaralhada:\n");
    embaralharFila(&fila);
    imprimirFila(&fila);

    Fila jogador1;
    inicializarFila(&jogador1);

    Fila jogador2;
    inicializarFila(&jogador2);

    distribuirPecas(&fila, &jogador1, &jogador2);

    printf("\nJogador 1:\n");
    imprimirFila(&jogador1);

    printf("\nJogador 2:\n");
    imprimirFila(&jogador2);

  /*  insertionSort(&jogador1);
    insertionSort(&jogador2);

   printf("\nJogador 1 (após a ordenação):\n");
    imprimirFila(&jogador1);

    printf("\nJogador 2 (após a ordenação):\n");
    imprimirFila(&jogador2); */
      
    Fila mesa;
    inicializarFila(&mesa);

   // verificarJogada(&jogador1, &jogador2, &mesa);
  
  //  printf("\nMesa:\n");
  //  imprimirFila(&mesa);
    
    return 0;
}
