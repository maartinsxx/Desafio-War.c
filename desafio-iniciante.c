#include <stdio.h>
#include <string.h>

/*
    Estrutura que representa um território no jogo War.
*/
typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

int main() {

    Territorio territorios[5];
    int i;  // <--- VARIÁVEL DECLARADA ANTES DO FOR (EVITA O ERRO)

    printf("=== SISTEMA DE CADASTRO DE TERRITORIOS (WAR) ===\n\n");

    for (i = 0; i < 5; i++) {
        printf("\nCadastro do Territorio %d:\n", i + 1);

        printf("Nome do territorio: ");
        scanf("%29s", territorios[i].nome);

        printf("Cor do exercito: ");
        scanf("%9s", territorios[i].cor);

        printf("Quantidade de tropas: ");
        scanf("%d", &territorios[i].tropas);
    }

    printf("\n=== TERRITORIOS CADASTRADOS ===\n\n");

    for (i = 0; i < 5; i++) {
        printf("Territorio %d:\n", i + 1);
        printf("Nome: %s\n", territorios[i].nome);
        printf("Cor do exercito: %s\n", territorios[i].cor);
        printf("Tropas: %d\n", territorios[i].tropas);
        printf("---------------------------\n");
    }

    return 0;
}
