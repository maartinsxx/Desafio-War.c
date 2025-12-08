#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_NOME 30
#define MAX_COR 10
#define TAM_MISSAO 150

/* ===========================================================
   ESTRUTURA DO TERRITÓRIO
   =========================================================== */
typedef struct {
    char nome[MAX_NOME];
    char cor[MAX_COR];
    int tropas;
} Territorio;

/* ===========================================================
   LIMPAR BUFFER DO TECLADO
   =========================================================== */
void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

/* ===========================================================
   EXIBIÇÃO DO MAPA
   =========================================================== */
void exibirMapa(Territorio* mapa, int qtd) {
    int i;
    printf("\n===== MAPA ATUAL =====\n");
    for (i = 0; i < qtd; i++) {
        printf("[%d] %s | Cor: %s | Tropas: %d\n",
               i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
    printf("=======================\n");
}

/* ===========================================================
   ATAQUE ENTRE TERRITÓRIOS
   =========================================================== */
void atacar(Territorio* atacante, Territorio* defensor) {
    int dadoA = (rand() % 6) + 1;
    int dadoD = (rand() % 6) + 1;

    printf("\nDado do Atacante (%s): %d\n", atacante->nome, dadoA);
    printf("Dado do Defensor (%s): %d\n", defensor->nome, dadoD);

    /* Atacante vence */
    if (dadoA > dadoD) {
        int tropasTransferidas = atacante->tropas / 2;
        if (tropasTransferidas < 1) tropasTransferidas = 1;

        defensor->tropas = tropasTransferidas;
        strcpy(defensor->cor, atacante->cor);
        atacante->tropas -= tropasTransferidas;

        printf("\n>>> Vitória do Atacante! %s conquistado!\n", defensor->nome);
    }
    /* Defensor vence */
    else {
        atacante->tropas -= 1;
        printf("\n>>> Defesa bem sucedida! %s perde 1 tropa.\n", atacante->nome);
    }

    if (atacante->tropas < 0) atacante->tropas = 0;
}

/* ===========================================================
   CADASTRO DO MAPA
   =========================================================== */
Territorio* cadastrarMapa(int qtd) {
    int i;
    Territorio* mapa = (Territorio*) calloc(qtd, sizeof(Territorio));

    if (mapa == NULL) {
        printf("Erro ao alocar mapa.\n");
        exit(1);
    }

    printf("\n=== Cadastro de Territórios ===\n");
    for (i = 0; i < qtd; i++) {
        printf("\nTerritório %d\n", i);

        printf("Nome: ");
        limparBuffer();
        fgets(mapa[i].nome, MAX_NOME, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor (ex: Vermelho, Azul): ");
        fgets(mapa[i].cor, MAX_COR, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Tropas: ");
        while (scanf("%d", &mapa[i].tropas) != 1 || mapa[i].tropas <= 0) {
            printf("Valor inválido. Digite novamente: ");
            limparBuffer();
        }
    }

    return mapa;
}

/* ===========================================================
   SORTEIO E ATRIBUIÇÃO DE MISSÃO
   =========================================================== */
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int sorteio = rand() % totalMissoes;
    strcpy(destino, missoes[sorteio]);
}

/* ===========================================================
   EXIBIR MISSÃO APENAS UMA VEZ
   =========================================================== */
void exibirMissao(char* missao) {
    printf("\n===== SUA MISSÃO =====\n");
    printf("%s\n", missao);
    printf("=======================\n\n");
}

/* ===========================================================
   VERIFICAÇÃO DE MISSÃO
   -> Lógica simples inicial (atende ao desafio)
   =========================================================== */
int verificarMissao(char* missao, Territorio* mapa, int qtd) {
    int i, count = 0;

    /* Missão: Conquistar 3 territórios seguidos */
    if (strstr(missao, "3 territorios")) {
        for (i = 0; i < qtd; i++) {
            if (mapa[i].tropas > 0) count++;
        }
        if (count >= 3) return 1;
    }

    /* Missão: Eliminar cor vermelha */
    if (strstr(missao, "vermelha")) {
        int existe = 0;
        for (i = 0; i < qtd; i++) {
            if (strcmp(mapa[i].cor, "Vermelho") == 0) existe = 1;
        }
        if (!existe) return 1;
    }

    return 0;
}

/* ===========================================================
   LIBERAÇÃO DE MEMÓRIA
   =========================================================== */
void liberarMemoria(Territorio* mapa, char* missaoJogador) {
    if (mapa != NULL) free(mapa);
    if (missaoJogador != NULL) free(missaoJogador);
}

/* ===========================================================
   FUNÇÃO PRINCIPAL
   =========================================================== */
int main() {
    int qtd, opcao;
    int atacante, defensor;

    /* Missões pré-definidas */
    char* missoes[] = {
        "Conquistar 3 territorios seguidos.",
        "Eliminar todas as tropas da cor vermelha.",
        "Manter 2 territorios sem perder tropas.",
        "Conquistar qualquer territorio azul.",
        "Terminar o turno com mais de 10 tropas."
    };
    int totalMissoes = 5;

    srand(time(NULL));

    printf("Quantidade de territorios: ");
    scanf("%d", &qtd);

    Territorio* mapa = cadastrarMapa(qtd);

    /* Missão do jogador alocada dinamicamente */
    char* missaoJogador = (char*) malloc(TAM_MISSAO);
    atribuirMissao(missaoJogador, missoes, totalMissoes);
    exibirMissao(missaoJogador); /* Exibe 1 vez */

    do {
        printf("\nMENU:\n");
        printf("1 - Atacar\n");
        printf("2 - Exibir mapa\n");
        printf("0 - Sair\n");
        printf("Escolha: ");
        scanf("%d", &opcao);

        if (opcao == 1) {
            exibirMapa(mapa, qtd);

            printf("Território atacante: ");
            scanf("%d", &atacante);

            printf("Território defensor: ");
            scanf("%d", &defensor);

            if (atacante == defensor) {
                printf("Não é possível atacar o mesmo território.\n");
                continue;
            }

            if (strcmp(mapa[atacante].cor, mapa[defensor].cor) == 0) {
                printf("Não é possível atacar um território da mesma cor.\n");
                continue;
            }

            atacar(&mapa[atacante], &mapa[defensor]);

            /* Verificação silenciosa */
            if (verificarMissao(missaoJogador, mapa, qtd)) {
                printf("\n=============================\n");
                printf(" PARABÉNS! VOCÊ CUMPRIU SUA MISSÃO!\n");
                printf(" MISSÃO: %s\n", missaoJogador);
                printf("=============================\n");
                break;
            }
        }

        else if (opcao == 2) {
            exibirMapa(mapa, qtd);
        }

    } while (opcao != 0);

    liberarMemoria(mapa, missaoJogador);
    return 0;
}
