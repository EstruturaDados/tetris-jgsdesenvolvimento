#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5
#define NUM_TIPOS_PECAS 4

// Estrutura para representar uma peça
typedef struct {
    char nome;
    int id;
} Peca;

// Estrutura para a fila circular
typedef struct {
    Peca pecas[TAMANHO_FILA];
    int frente;
    int tras;
    int quantidade;
} FilaCircular;

// Protótipos das funções
void inicializarFila(FilaCircular *fila);
Peca gerarPeca();
void enqueue(FilaCircular *fila, Peca peca);
Peca dequeue(FilaCircular *fila);
void visualizarFila(FilaCircular *fila);
void mostrarMenu();

int main() {
    srand(time(NULL)); // Inicializa a semente para números aleatórios
    
    FilaCircular fila;
    inicializarFila(&fila);
    
    int opcao;
    
    printf("=== Tetris Stack - Nível Novato ===\n");
    printf("Sistema de Fila de Peças Futuras\n\n");
    
    do {
        mostrarMenu();
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1:
                visualizarFila(&fila);
                break;
                
            case 2:
                if (fila.quantidade > 0) {
                    Peca pecaJogada = dequeue(&fila);
                    printf("Peça jogada: %c (ID: %d)\n", pecaJogada.nome, pecaJogada.id);
                    
                    // Gera e insere nova peça automaticamente
                    Peca novaPeca = gerarPeca();
                    enqueue(&fila, novaPeca);
                    printf("Nova peça inserida: %c (ID: %d)\n", novaPeca.nome, novaPeca.id);
                    
                    visualizarFila(&fila);
                } else {
                    printf("Fila vazia! Não há peças para jogar.\n");
                }
                break;
                
            case 3: {
                Peca novaPeca = gerarPeca();
                enqueue(&fila, novaPeca);
                printf("Nova peça inserida: %c (ID: %d)\n", novaPeca.nome, novaPeca.id);
                visualizarFila(&fila);
                break;
            }
                
            case 0:
                printf("Saindo do sistema...\n");
                break;
                
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
        printf("\n");
        
    } while (opcao != 0);
    
    return 0;
}

// Inicializa a fila com 5 peças geradas automaticamente
void inicializarFila(FilaCircular *fila) {
    fila->frente = 0;
    fila->tras = -1;
    fila->quantidade = 0;
    
    for (int i = 0; i < TAMANHO_FILA; i++) {
        Peca novaPeca = gerarPeca();
        enqueue(fila, novaPeca);
    }
    
    printf("Fila inicializada com 5 peças:\n");
    visualizarFila(fila);
    printf("\n");
}

// Gera uma peça aleatória
Peca gerarPeca() {
    char tipos[] = {'I', 'O', 'T', 'L'};
    Peca peca;
    
    peca.nome = tipos[rand() % NUM_TIPOS_PECAS];
    peca.id = rand() % 1000; // ID entre 0 e 999
    
    return peca;
}

// Insere uma peça no final da fila
void enqueue(FilaCircular *fila, Peca peca) {
    if (fila->quantidade == TAMANHO_FILA) {
        printf("Fila cheia! Não é possível inserir mais peças.\n");
        return;
    }
    
    fila->tras = (fila->tras + 1) % TAMANHO_FILA;
    fila->pecas[fila->tras] = peca;
    fila->quantidade++;
}

// Remove e retorna a peça da frente da fila
Peca dequeue(FilaCircular *fila) {
    if (fila->quantidade == 0) {
        printf("Fila vazia!\n");
        Peca pecaVazia = {' ', -1};
        return pecaVazia;
    }
    
    Peca peca = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % TAMANHO_FILA;
    fila->quantidade--;
    
    return peca;
}

// Exibe o estado atual da fila
void visualizarFila(FilaCircular *fila) {
    printf("=== FILA ATUAL (%d/%d peças) ===\n", fila->quantidade, TAMANHO_FILA);
    
    if (fila->quantidade == 0) {
        printf("Fila vazia!\n");
        return;
    }
    
    int indice = fila->frente;
    for (int i = 0; i < fila->quantidade; i++) {
        printf("[%d] %c (ID: %d)\n", i + 1, fila->pecas[indice].nome, fila->pecas[indice].id);
        indice = (indice + 1) % TAMANHO_FILA;
    }
}

// Mostra o menu de opções
void mostrarMenu() {
    printf("=== MENU ===\n");
    printf("1. Visualizar fila\n");
    printf("2. Jogar peça (remover da frente)\n");
    printf("3. Inserir nova peça\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}