/*
  War Estruturado - Módulo de Ataque entre Territórios
  Requisitos atendidos:
    - Uso de malloc/calloc para criar vetor dinâmico de Territorio
    - Acesso e modificação via ponteiros
    - Função atacar(Territorio* atacante, Territorio* defensor) usando rand() para simular dados (1-6)
    - Transferência de cor e metade das tropas ao defensor em caso de vitória do atacante
    - Perda de 1 tropa do atacante em caso de derrota
    - Modularização: cadastro, exibição, ataque, liberação de memória
    - srand(time(NULL)) para aleatoriedade
    - Validações e interface amigável no terminal
*
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 30
#define MAX_COR 10

typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

/* Função: limparBuffer
   Limpa o restante do buffer de entrada (stdin) para evitar problemas com fgets/scanf. */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

/* Função: cadastroTerritorios
   Aloca dinamicamente (calloc) e preenche o vetor de territórios.
   Retorna o ponteiro para o vetor (Territorio*). */
Territorio* cadastroTerritorios(int qtd) {
    Territorio* mapa = (Territorio*) calloc(qtd, sizeof(Territorio));
    if (mapa == NULL) {
        fprintf(stderr, "Erro ao alocar memoria para os territorios.\n");
        exit(EXIT_FAILURE);
    }

    printf("\n=== Cadastro de Territorios ===\n");
    for (int i = 0; i < qtd; i++) {
        printf("\nTerritorio %d:\n", i);
        printf("Nome (max %d chars): ", MAX_NOME - 1);
        // lê linha com espaços
        if (fgets(mapa[i].nome, MAX_NOME, stdin) == NULL) {
            strcpy(mapa[i].nome, "Territorio");
        } else {
            // remove newline
            size_t ln = strlen(mapa[i].nome);
            if (ln > 0 && mapa[i].nome[ln-1] == '\n') mapa[i].nome[ln-1] = '\0';
            if (strlen(mapa[i].nome) == 0) sprintf(mapa[i].nome, "Territorio_%d", i);
        }

        printf("Cor do exército (ex: Vermelho, Azul) (max %d chars): ", MAX_COR - 1);
        if (fgets(mapa[i].cor, MAX_COR, stdin) == NULL) {
            strcpy(mapa[i].cor, "Neutra");
        } else {
            size_t ln = strlen(mapa[i].cor);
            if (ln > 0 && mapa[i].cor[ln-1] == '\n') mapa[i].cor[ln-1] = '\0';
            if (strlen(mapa[i].cor) == 0) sprintf(mapa[i].cor, "Neutra");
        }

        printf("Numero de tropas (inteiro > 0): ");
        while (scanf("%d", &mapa[i].tropas) != 1 || mapa[i].tropas <= 0) {
            printf("Entrada invalida. Digite um inteiro positivo para tropas: ");
            limparBuffer();
        }
        limparBuffer();
    }

    return mapa;
}

/* Função: exibirTerritorios
   Mostra no terminal os territórios com índice, nome, cor e tropas.
   Usa ponteiro para o vetor. */
void exibirTerritorios(Territorio* mapa, int qtd) {
    printf("\n--- Mapa Atual (%d territorios) ---\n", qtd);
    for (int i = 0; i < qtd; i++) {
        printf("[%d] %s | Cor: %s | Tropas: %d\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("-------------------------------\n");
}

/* Função: atacar
   Simula um ataque entre dois territórios:
     - Gera um dado aleatório (1..6) para atacante e para defensor
     - Se atacante ganhar (valor > defensor), transfere cor e metade das tropas do atacante
       para o defensor (arredonda para baixo em caso de impar)
     - Se atacante perder ou empatar, atacante perde 1 tropa
   Todos os acessos são feitos por ponteiros.
*/
void atacar(Territorio* atacante, Territorio* defensor) {
    if (atacante == NULL || defensor == NULL) return;

    // Validações básicas (assumindo que isso será checado antes, mas reforça aqui)
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Ataque invalido: territorios possuem a mesma cor (mesmo dono).\n");
        return;
    }
    if (atacante->tropas <= 1) {
        printf("Ataque invalido: o territorio atacante precisa ter mais de 1 tropa para atacar.\n");
        return;
    }

    int dado_atacante = (rand() % 6) + 1; // 1..6
    int dado_defensor  = (rand() % 6) + 1; // 1..6

    printf("\nRolagem de dados: Atacante(%s) = %d | Defensor(%s) = %d\n",
           atacante->nome, dado_atacante, defensor->nome, dado_defensor);

    if (dado_atacante > dado_defensor) {
        // Atacante vence
        int tropas_transferidas = atacante->tropas / 2; // metade, arredondando para baixo
        if (tropas_transferidas < 1) tropas_transferidas = 1; // garante pelo menos 1 tropa transferida

        // Atualiza defensor: recebe cor e tropas transferidas
        strncpy(defensor->cor, atacante->cor, MAX_COR - 1);
        defensor->cor[MAX_COR - 1] = '\0';
        defensor->tropas = tropas_transferidas;

        // Atualiza atacante: perde as tropas transferidas
        atacante->tropas -= tropas_transferidas;
        if (atacante->tropas < 0) atacante->tropas = 0;

        printf("Resultado: Atacante VENCEU! %d tropas transferidas para %s. %s agora pertence a %s.\n",
               tropas_transferidas, defensor->nome, defensor->nome, defensor->cor);
    } else {
        // Empate ou defensor ganha -> atacante perde 1 tropa
        atacante->tropas -= 1;
        if (atacante->tropas < 0) atacante->tropas = 0;
        printf("Resultado: Atacante PERDEU ou houve EMPATE. %s perde 1 tropa.\n", atacante->nome);
    }
}

/* Função: liberarMemoria
   Libera a memória alocada para o vetor de territorios. */
void liberarMemoria(Territorio* mapa) {
    if (mapa != NULL) {
        free(mapa);
    }
}

/* Função: selecionarIndiceValido
   Lê do usuário um índice entre 0 e max-1. */
int selecionarIndiceValido(int max, const char* prompt) {
    int idx;
    printf("%s (0 a %d): ", prompt, max - 1);
    while (scanf("%d", &idx) != 1 || idx < 0 || idx >= max) {
        printf("Indice invalido. %s (0 a %d): ", prompt, max - 1);
        limparBuffer();
    }
    limparBuffer();
    return idx;
}

/* Função principal: menu e loop do programa */
int main() {
    srand((unsigned int) time(NULL)); // inicializa semente dos numeros aleatorios

    printf("=== WAR Estruturado: Simulador de Ataques entre Territorios ===\n");

    int qtd;
    printf("Digite o numero total de territorios: ");
    while (scanf("%d", &qtd) != 1 || qtd <= 0) {
        printf("Entrada invalida. Digite um inteiro positivo para o numero de territorios: ");
        limparBuffer();
    }
    limparBuffer();

    // Cadastro (alocação dinâmica)
    Territorio* mapa = cadastroTerritorios(qtd);

    int opcao;
    do {
        exibirTerritorios(mapa, qtd);
        printf("\nMenu:\n");
        printf("1 - Efetuar um ataque\n");
        printf("2 - Exibir territorios\n");
        printf("3 - Re-cadastrar (sobrescrever) territorios\n");
        printf("0 - Sair\n");
        printf("Escolha uma opcao: ");
        while (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida. Digite a opcao: ");
            limparBuffer();
        }
        limparBuffer();

        if (opcao == 1) {
            // Seleciona indices
            int idxAtq = selecionarIndiceValido(qtd, "Escolha o indice do TERRITORIO ATACANTE");
            int idxDef = selecionarIndiceValido(qtd, "Escolha o indice do TERRITORIO DEFENSOR");

            // Valida que não atacará a si mesmo e que não seja mesmo dono
            if (idxAtq == idxDef) {
                printf("Erro: nao e possivel atacar o mesmo territorio.\n");
                continue;
            }

            Territorio* atacante = &mapa[idxAtq];
            Territorio* defensor  = &mapa[idxDef];

            if (strcmp(atacante->cor, defensor->cor) == 0) {
                printf("Erro: nao e possivel atacar um territorio da mesma cor/dono.\n");
                continue;
            }

            if (atacante->tropas <= 1) {
                printf("Erro: o territorio atacante precisa ter mais de 1 tropa para iniciar o ataque.\n");
                continue;
            }

            // Executa ataque
            atacar(atacante, defensor);

            // Exibe mapa atualizado após ataque
            exibirTerritorios(mapa, qtd);

        } else if (opcao == 2) {
            exibirTerritorios(mapa, qtd);
        } else if (opcao == 3) {
            printf("Re-cadastrando todos os territorios (sobrescrevendo)...\n");
            liberarMemoria(mapa);
            mapa = cadastroTerritorios(qtd);
        } else if (opcao == 0) {
            printf("Saindo... liberando memoria.\n");
        } else {
            printf("Opcao invalida.\n");
        }

    } while (opcao != 0);

    liberarMemoria(mapa);
    return 0;
}
