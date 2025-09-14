#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define TAMANHO_FILA 5
#define TAMANHO_PILHA 3
#define NUM_TIPOS_PECAS 4
#define MAX_HISTORICO 10

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

// Estrutura para o histórico de ações (para desfazer)
typedef struct {
    int tipo_acao; // 1: Jogar, 2: Reservar, 3: Usar, 4: Trocar, 5: Inverter
    Peca peca_afetada;
    int origem; // 0: Fila, 1: Pilha
    int posicao;
} AcaoHistorico;

// Estrutura para a pilha de histórico
typedef struct {
    AcaoHistorico acoes[MAX_HISTORICO];
    int topo;
    int quantidade;
} PilhaHistorico;

// Protótipos das funções
void inicializarFila(FilaCircular *fila);
void inicializarPilha(PilhaReserva *pilha);
void inicializarHistorico(PilhaHistorico *historico);
Peca gerarPeca();
void enqueue(FilaCircular *fila, Peca peca);
Peca dequeue(FilaCircular *fila);
int push(PilhaReserva *pilha, Peca peca);
Peca pop(PilhaReserva *pilha);
void registrarHistorico(PilhaHistorico *historico, int tipo_acao, Peca peca_afetada, int origem, int posicao);
int trocarPecas(FilaCircular *fila, PilhaReserva *pilha, PilhaHistorico *historico);
int desfazerUltimaAcao(FilaCircular *fila, PilhaReserva *pilha, PilhaHistorico *historico);
int inverterFilaComPilha(FilaCircular *fila, PilhaReserva *pilha, PilhaHistorico *historico);
void visualizarFila(FilaCircular *fila);
void visualizarPilha(PilhaReserva *pilha);
void mostrarMenu();

int main() {
    srand(time(NULL)); // Inicializa a semente para números aleatórios
    
    FilaCircular fila;
    PilhaReserva pilha;
    PilhaHistorico historico;
    
    inicializarFila(&fila);
    inicializarPilha(&pilha);
    inicializarHistorico(&historico);
    
    int opcao;
    
    printf("=== Tetris Stack - Nível Mestre ===\n");
    printf("Sistema Integrado com Estratégias Avançadas\n\n");
    
    do {
        mostrarMenu();
        scanf("%d", &opcao);
        
        switch(opcao) {
            case 1: // Jogar peça
                if (fila.quantidade > 0) {
                    Peca pecaJogada = dequeue(&fila);
                    printf("Peça jogada: %c (ID: %d)\n", pecaJogada.nome, pecaJogada.id);
                    
                    // Registra no histórico
                    registrarHistorico(&historico, 1, pecaJogada, 0, fila.frente);
                    
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
                            
                            // Registra no histórico
                            registrarHistorico(&historico, 2, pecaReservada, 0, fila.frente);
                            
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
                    
                    // Registra no histórico
                    registrarHistorico(&historico, 3, pecaUsada, 1, pilha.topo + 1);
                    
                    // A fila permanece cheia (não insere nova peça ao usar reserva)
                    printf("Peça da reserva utilizada!\n");
                } else {
                    printf("Pilha de reserva vazia! Não há peças reservadas.\n");
                }
                break;
                
            case 4: // Trocar peça do topo da pilha com a da frente da fila
                if (trocarPecas(&fila, &pilha, &historico)) {
                    printf("Troca realizada com sucesso!\n");
                } else {
                    printf("Não foi possível realizar a troca.\n");
                }
                break;
                
            case 5: // Desfazer última jogada
                if (desfazerUltimaAcao(&fila, &pilha, &historico)) {
                    printf("Última ação desfeita com sucesso!\n");
                } else {
                    printf("Não há ações para desfazer.\n");
                }
                break;
                
            case 6: // Inverter fila com pilha
                if (inverterFilaComPilha(&fila, &pilha, &historico)) {
                    printf("Inversão realizada com sucesso!\n");
                } else {
                    printf("Não foi possível realizar a inversão.\n");
                }
                break;
                
            case 7: // Visualizar estado atual
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
        if (opcao != 7 && opcao != 0) {
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
    printf("Pilha de reserva inicializada (capacidade: %d)\n", TAMANHO_PILHA);
}

// Inicializa o histórico vazio
void inicializarHistorico(PilhaHistorico *historico) {
    historico->topo = -1;
    historico->quantidade = 0;
    printf("Sistema de histórico inicializado (capacidade: %d)\n\n", MAX_HISTORICO);
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

// Registra uma ação no histórico
void registrarHistorico(PilhaHistorico *historico, int tipo_acao, Peca peca_afetada, int origem, int posicao) {
    if (historico->quantidade == MAX_HISTORICO) {
        // Remove a ação mais antiga (implementação FIFO para histórico)
        for (int i = 0; i < MAX_HISTORICO - 1; i++) {
            historico->acoes[i] = historico->acoes[i + 1];
        }
        historico->quantidade--;
        historico->topo--;
    }
    
    AcaoHistorico novaAcao;
    novaAcao.tipo_acao = tipo_acao;
    novaAcao.peca_afetada = peca_afetada;
    novaAcao.origem = origem;
    novaAcao.posicao = posicao;
    
    historico->topo++;
    historico->acoes[historico->topo] = novaAcao;
    historico->quantidade++;
}

// Troca a peça do topo da pilha com a da frente da fila
int trocarPecas(FilaCircular *fila, PilhaReserva *pilha, PilhaHistorico *historico) {
    if (fila->quantidade == 0 || pilha->quantidade == 0) {
        printf("É necessário ter peças na fila e na pilha para trocar.\n");
        return 0;
    }
    
    // Remove a peça da frente da fila
    Peca pecaFila = dequeue(fila);
    
    // Remove a peça do topo da pilha
    Peca pecaPilha = pop(pilha);
    
    // Insere a peça da pilha na fila
    enqueue(fila, pecaPilha);
    
    // Insere a peça da fila na pilha
    push(pilha, pecaFila);
    
    // Registra ambas as ações no histórico
    registrarHistorico(historico, 4, pecaFila, 0, fila->frente);
    registrarHistorico(historico, 4, pecaPilha, 1, pilha->topo);
    
    printf("Peça %c (ID: %d) da fila trocada com peça %c (ID: %d) da pilha.\n",
           pecaFila.nome, pecaFila.id, pecaPilha.nome, pecaPilha.id);
    
    return 1;
}

// Desfaz a última ação realizada
int desfazerUltimaAcao(FilaCircular *fila, PilhaReserva *pilha, PilhaHistorico *historico) {
    if (historico->quantidade == 0) {
        return 0;
    }
    
    AcaoHistorico ultimaAcao = historico->acoes[historico->topo];
    historico->topo--;
    historico->quantidade--;
    
    switch (ultimaAcao.tipo_acao) {
        case 1: // Desfazer jogar peça
            // Remove a peça recém-inserida
            dequeue(fila);
            
            // Reinsere a peça jogada na frente
            fila->frente = (fila->frente - 1 + TAMANHO_FILA) % TAMANHO_FILA;
            fila->pecas[fila->frente] = ultimaAcao.peca_afetada;
            fila->quantidade++;
            
            printf("Jogada da peça %c (ID: %d) desfeita.\n", 
                   ultimaAcao.peca_afetada.nome, ultimaAcao.peca_afetada.id);
            break;
            
        case 2: // Desfazer reservar peça
            // Remove a peça recém-inserida na fila
            dequeue(fila);
            
            // Remove a peça reservada da pilha
            pop(pilha);
            
            // Reinsere a peça reservada na frente da fila
            fila->frente = (fila->frente - 1 + TAMANHO_FILA) % TAMANHO_FILA;
            fila->pecas[fila->frente] = ultimaAcao.peca_afetada;
            fila->quantidade++;
            
            printf("Reserva da peça %c (ID: %d) desfeita.\n", 
                   ultimaAcao.peca_afetada.nome, ultimaAcao.peca_afetada.id);
            break;
            
        case 3: // Desfazer usar peça reservada
            // Reinsere a peça usada no topo da pilha
            push(pilha, ultimaAcao.peca_afetada);
            
            printf("Uso da peça reservada %c (ID: %d) desfeita.\n", 
                   ultimaAcao.peca_afetada.nome, ultimaAcao.peca_afetada.id);
            break;
            
        case 4: // Desfazer troca (faz a troca inversa)
            // Para simplificar, vamos fazer a troca novamente
            trocarPecas(fila, pilha, historico);
            // Remove a ação de troca do histórico (pois estamos desfazendo)
            historico->topo--;
            historico->quantidade--;
            
            printf("Troca de peças desfeita.\n");
            break;
            
        case 5: // Desfazer inversão
            // Para simplificar, vamos inverter novamente
            inverterFilaComPilha(fila, pilha, historico);
            // Remove a ação de inversão do histórico (pois estamos desfazendo)
            historico->topo--;
            historico->quantidade--;
            
            printf("Inversão desfeita.\n");
            break;
    }
    
    return 1;
}

// Inverte a ordem da fila usando a pilha como auxiliar
int inverterFilaComPilha(FilaCircular *fila, PilhaReserva *pilha, PilhaHistorico *historico) {
    if (fila->quantidade == 0) {
        printf("Fila vazia! Não é possível inverter.\n");
        return 0;
    }
    
    // Registra o estado atual para possível desfazer
    for (int i = 0; i < fila->quantidade; i++) {
        int posicao = (fila->frente + i) % TAMANHO_FILA;
        registrarHistorico(historico, 5, fila->pecas[posicao], 0, posicao);
    }
    
    // Usa a pilha como auxiliar para inverter a fila
    PilhaReserva pilhaAuxiliar;
    inicializarPilha(&pilhaAuxiliar);
    
    // Transfere todas as peças da fila para a pilha auxiliar
    while (fila->quantidade > 0) {
        Peca peca = dequeue(fila);
        push(&pilhaAuxiliar, peca);
    }
    
    // Transfere todas as peças de volta para a fila (agora invertidas)
    while (pilhaAuxiliar.quantidade > 0) {
        Peca peca = pop(&pilhaAuxiliar);
        enqueue(fila, peca);
    }
    
    printf("Fila invertida com sucesso usando a pilha como auxiliar.\n");
    return 1;
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
    printf("=== MENU PRINCIPAL ===\n");
    printf("1. Jogar peça\n");
    printf("2. Reservar peça\n");
    printf("3. Usar peça reservada\n");
    printf("4. Trocar peça (fila <-> pilha)\n");
    printf("5. Desfazer última ação\n");
    printf("6. Inverter fila com pilha\n");
    printf("7. Visualizar estado atual\n");
    printf("0. Sair\n");
    printf("Escolha uma opção: ");
}