#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3
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

// Estrutura para a pilha de reserva
typedef struct {
    Peca pecas[TAMANHO_PILHA];
    int topo;
    int quantidade;
} PilhaReserva;

// Protótipos das funções
void inicializarFila(FilaCircular *fila);
void inicializarPilha(PilhaReserva *pilha);
Peca gerarPeca();
void enqueue(FilaCircular *fila, Peca peca);
Peca dequeue(FilaCircular *fila);
int push(PilhaReserva *pilha, Peca peca);
Peca pop(PilhaReserva *pilha);
void visualizarFila(FilaCircular *fila);
void visualizarPilha(PilhaReserva *pilha);
void mostrarMenu();

int main() {
    srand(time(NULL)); // Inicializa a semente para números aleatórios
    
    FilaCircular fila;
    PilhaReserva pilha;
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    
    int opcao;
    
    printf("=== Tetris Stack - Nível Aventureiro ===\n");
    printf("Sistema de Fila e Pilha de Reserva\n\n");
    
    do {
        mostrarMenu();
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1: // Jogar peça
                if (fila.quantidade > 0) {
                    Peca pecaJogada = dequeue(&fila);
                    printf("Peça jogada: %c (ID: %d)\n", pecaJogada.nome, pecaJogada.id);
                    
                    // Gera e insere nova peça automaticamente
                    Peca novaPeca = gerarPeca();
                    enqueue(&fila, novaPeca);
                    printf("Nova peça inserida: %c (ID: %d)\n", novaPeca.nome, novaPeca.id);
                } else {
                    printf("Fila vazia! Não há peças para jogar.\n");
                }
                break;
                
            case 2: // Reservar peça
                if (fila.quantidade > 0) {
                    if (pilha.quantidade < TAMANHO_PILHA) {
                        Peca pecaReservada = dequeue(&fila);
                        if (push(&pilha, pecaReservada)) {
                            printf("Peça reservada: %c (ID: %d)\n", pecaReservada.nome, pecaReservada.id);
                            
                            // Gera e insere nova peça automaticamente
                            Peca novaPeca = gerarPeca();
                            enqueue(&fila, novaPeca);
                            printf("Nova peça inserida: %c (ID: %d)\n", novaPeca.nome, novaPeca.id);
                        }
                    } else {
                        printf("Pilha de reserva cheia! Não é possível reservar mais peças.\n");
                    }
                } else {
                    printf("Fila vazia! Não há peças para reservar.\n");
                }
                break;
                
            case 3: // Usar peça reservada
                if (pilha.quantidade > 0) {
                    Peca pecaUsada = pop(&pilha);
                    printf("Peça usada da reserva: %c (ID: %d)\n", pecaUsada.nome, pecaUsada.id);
                    
                    // A fila permanece cheia (não insere nova peça ao usar reserva)
                    printf("Peça da reserva utilizada!\n");
                } else {
                    printf("Pilha de reserva vazia! Não há peças reservadas.\n");
                }
                break;
                
            case 4: // Visualizar estado atual
                visualizarFila(&fila);
                visualizarPilha(&pilha);
                break;
                
            case 0: // Sair
                printf("Saindo do sistema...\n");
                break;
                
            default:
                printf("Opção inválida! Tente novamente.\n");
        }
        
        // Mostra o estado atual após cada operação (exceto visualizar)
        if (opcao != 4 && opcao != 0) {
            printf("\n");
            visualizarFila(&fila);
            visualizarPilha(&pilha);
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
}

// Inicializa a pilha de reserva vazia
void inicializarPilha(PilhaReserva *pilha) {
    pilha->topo = -1;
    pilha->quantidade = 0;
    printf("Pilha de reserva inicializada (capacidade: %d)\n\n", TAMANHO_PILHA);
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

// Insere uma peça no topo da pilha (reserva)
int push(PilhaReserva *pilha, Peca peca) {
    if (pilha->quantidade == TAMANHO_PILHA) {
        return 0; // Falha - pilha cheia
    }
    
    pilha->topo++;
    pilha->pecas[pilha->topo] = peca;
    pilha->quantidade++;
    
    return 1; // Sucesso
}

// Remove e retorna a peça do topo da pilha (uso da reserva)
Peca pop(PilhaReserva *pilha) {
    if (pilha->quantidade == 0) {
        printf("Pilha vazia!\n");
        Peca pecaVazia = {' ', -1};
        return pecaVazia;
    }
    
    Peca peca = pilha->pecas[pilha->topo];
    pilha->topo--;
    pilha->quantidade--;
    
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

// Exibe o estado atual da pilha de reserva
void visualizarPilha(PilhaReserva *pilha) {
    printf("=== PILHA DE RESERVA (%d/%d peças) ===\n", pilha->quantidade, TAMANHO_PILHA);
    
    if (pilha->quantidade == 0) {
        printf("Pilha vazia!\n");
        return;
    }
    
    for (int i = pilha->topo; i >= 0; i--) {
        printf("[%d] %c (ID: %d) %s\n", 
               pilha->topo - i + 1, 
               pilha->pecas[i].nome, 
               pilha->pecas[i].id,
               (i == pilha->topo) ? "← TOPO" : "");
    }
}

// Mostra o menu de opções
void mostrarMenu() {
    printf("=== MENU ===\n");
    printf("1. Jogar peça\n");
    printf("2. Reservar peça\n");
    printf("3. Usar peça reservada\n");
    printf("4. Visualizar estado atual\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}