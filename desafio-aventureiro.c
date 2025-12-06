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
} Território;

/* Função: LimparBuffer
   Limpa o restante do buffer da entrada (stdin) para evitar problemas com fgets/scanf.)

void LimparBuffer
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
    }
    
/* Função: cadastroTerritorios 
   Aloca dinamicamente (Calloc) e preenche o vetor de territórios.
