#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

void cadastrarTerritorios(Territorio* mapa, int n) {
    for (int i = 0; i < n; i++) {
        printf("\n--- Cadastro do Território %d ---\n", i + 1);
        printf("Nome: ");
        scanf(" %[^\n]", mapa[i].nome);
        printf("Cor do exército: ");
        scanf(" %[^\n]", mapa[i].cor);
        printf("Quantidade de tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

// Exibir todos os territórios
void exibirTerritorios(Territorio* mapa, int n) {
    printf("\n--- ESTADO ATUAL DOS TERRITÓRIOS ---\n");
    for (int i = 0; i < n; i++) {
        printf("Território %d: %s | Cor: %s | Tropas: %d\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Função de ataque
void atacar(Territorio* atacante, Territorio* defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) {
        printf("Erro: não é permitido atacar territórios da mesma cor!\n");
        return;
    }

    if (atacante->tropas < 2) {
        printf("Erro: é necessário ter pelo menos 2 tropas para atacar!\n");
        return;
    }

    // Simulação de dados
    int dadoAtacante = (rand() % 6) + 1;
    int dadoDefensor = (rand() % 6) + 1;

    printf("\nDado do Atacante (%s): %d\n", atacante->nome, dadoAtacante);
    printf("Dado do Defensor (%s): %d\n", defensor->nome, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf("Resultado: Atacante venceu!\n");
        strcpy(defensor->cor, atacante->cor); 
        defensor->tropas = atacante->tropas / 2; 
        atacante->tropas -= defensor->tropas; 
    } else {
        printf("Resultado: Defensor resistiu!\n");
        atacante->tropas -= 1; 
    }
}

// Liberar memória
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}

// Menu de ataque
void menuAtaque(Territorio* mapa, int n) {
    int idAtacante, idDefensor;

    printf("\n--- ATAQUE ENTRE TERRITÓRIOS ---\n");
    exibirTerritorios(mapa, n);

    printf("\nEscolha o ID do território ATACANTE: ");
    scanf("%d", &idAtacante);
    printf("Escolha o ID do território DEFENSOR: ");
    scanf("%d", &idDefensor);

    if (idAtacante < 0 || idAtacante >= n || idDefensor < 0 || idDefensor >= n) {
        printf("IDs inválidos! Operação cancelada.\n");
        return;
    }

    atacar(&mapa[idAtacante], &mapa[idDefensor]);
    exibirTerritorios(mapa, n);
}


int main() {
    srand(time(NULL)); 

    int total;
    printf("Digite o número de territórios: ");
    scanf("%d", &total);


    Territorio* mapa = (Territorio*) malloc(total * sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro ao alocar memória!\n");
        return 1;
    }

    cadastrarTerritorios(mapa, total);
    
    char continuar;
    do {
        menuAtaque(mapa, total);
        printf("\nDeseja realizar outro ataque? (s/n): ");
        scanf(" %c", &continuar);
    } while (continuar == 's' || continuar == 'S');

    liberarMemoria(mapa);
    printf("\nMemória liberada. Programa finalizado.\n");
    return 0;
}
