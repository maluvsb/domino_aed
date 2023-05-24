#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct {
    int pLado;
    int sLado;
} PecaDomino;

typedef struct Node {
    PecaDomino peca;
    struct Node* proximo;
    struct Node* posicao;
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
        printf("Peça %d: %d|%d\n", contador, nodeAtual->peca.pLado, nodeAtual->peca.sLado);
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

void distribuirPecas(Fila* fila, Fila* jogador1, Fila* jogador2) {
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

int somaLadosPeca(PecaDomino peca) {
    return peca.pLado + peca.sLado;
}

/* void insertionSort(Fila* jogador) {
    if (filaVazia(jogador) || jogador->frente->proximo == NULL) {
        return;
    }

    Node* atual = jogador->frente->proximo;

    while (atual != NULL) {
        PecaDomino peca = atual->peca;
        int somaPeca = somaLadosPeca(peca);
        Node* posicao = jogador->frente;
        Node* anterior = NULL;

        while (posicao != atual && somaLadosPeca(posicao->peca) > somaPeca) {
            anterior = posicao;
            posicao = posicao->proximo;
        }

        if (posicao != atual) {
            // Remove o nó atual da posição atual
            anterior->proximo = atual->proximo;

            // Insere o nó atual na nova posiçã
            if (anterior != NULL) {
                anterior->proximo = atual;
            } else {
                jogador->frente = atual;
            }

            atual->proximo = posicao;
        }

        atual = atual->proximo;
    }
} */


//  void verificarJogada(Fila* jogador1, Fila* jogador2, Fila* mesa) {
  //  Node* nodeAtualJogador1 = jogador1->frente;
    //Node* nodeAtualJogador2 = jogador2->frente;
    //PecaDomino pecaJogada;

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
    int escolha;
    
    Fila fila;
    inicializarFila(&fila);

    int contador = 0;
    for (int i = 0; i <= 6; i++) {
        for (int j = i; j <= 6; j++) {
            PecaDomino peca;
            peca.pLado = i;
            peca.sLado = j;
            enfileirar(&fila, peca);
            contador++;
        }
    }

    embaralharFila(&fila);

    Fila jogador1;
    inicializarFila(&jogador1);

    Fila jogador2;
    inicializarFila(&jogador2);
  
    do{
      printf(" -------------Menu------------- \n\n"); // comente o menu inteiro para executar as funções
      printf(" 1. Mostrar fila com pedras embaralhadas\n");
      printf(" 2. Iniciar Jogo\n");
      printf(" 3. Mostrar o dorme\n");
      printf(" 4. Mostrar o placar\n");
      printf(" 5. Encerrar Jogo\n");
      printf("\nDigite o número da opção desejada: ");
      scanf("%d", &escolha);

      switch (escolha) {
        
        case 1:
          printf("\nPedras embaralhadas:\n");
          imprimirFila(&fila);
        break;
        
        case 2:
          distribuirPecas(&fila, &jogador1, &jogador2);
          
          printf("\nJogador 1:\n");
          imprimirFila(&jogador1);

          printf("\nJogador 2:\n");
          imprimirFila(&jogador2);

          printf("\nOrdenando as pedras dos jogadores 1 e 2...\n");

          printf("\nPedras dos jogadores 1 e 2 ordenadas: \n");

    /*      printf("\nJogador 1: \n");
          insertionSort(&jogador1);
          imprimirFila(&jogador1);
          
          
          printf("\nJogador 2: \n");
          insertionSort(&jogador2);
          imprimirFila(&jogador2);      */
         break;

        case 3:
          printf("\nDorme: \n");
          imprimirFila(&fila);
        break;

        case 4:
          printf("implementar a funcao de mostrar placar\n");
        break;

        case 5:
          printf("\nSaindo do Jogo\n");
          return 0;
        break;
        
        default:
          printf("\nOpção inválida, digite outro número!\n");
        break;
      }

      printf("\n");
      
    }while(escolha != 5);  

   // Fila mesa;
   // inicializarFila(&mesa);

   // verificarJogada(&jogador1, &jogador2, &mesa);
  
  //  printf("\nMesa:\n");
  //  imprimirFila(&mesa);
    
    return 0;
}
