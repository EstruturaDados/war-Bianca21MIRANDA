#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TERRITORIOS 10
#define MAX_MISSAO 256
#define MAX_JOGADORES 2

// Struct para representar um território
typedef struct {
    char nome[30];
    char cor[10]; // ex: "vermelho", "azul"
    int tropas;
} Territorio;

// ======== PROTÓTIPOS =========
void exibirMapa(Territorio* mapa, int tamanho);
void atacar(Territorio* atacante, Territorio* defensor);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho, const char* corJogador);
void exibirMissao(const char* missao);
void liberarMemoria(char** missoesJogadores, Territorio* mapa);

// ======== MAIN =========
int main() {
    srand(time(NULL));

    // Missões disponíveis
    char* missoesDisponiveis[] = {
        "Conquistar 3 territorios seguidos",
        "Eliminar todas as tropas da cor vermelha",
        "Controlar 2 territorios com pelo menos 5 tropas",
        "Conquistar o territorio 'Amazonas'",
        "Eliminar todas as tropas da cor azul"
    };
    int totalMissoes = 5;

    // Alocar dinamicamente as missões dos jogadores
    char* missoesJogadores[MAX_JOGADORES];
    for (int i = 0; i < MAX_JOGADORES; i++) {
        missoesJogadores[i] = (char*)malloc(MAX_MISSAO * sizeof(char));
        atribuirMissao(missoesJogadores[i], missoesDisponiveis, totalMissoes);
    }

    // Criar o mapa com territórios
    Territorio* mapa = (Territorio*)malloc(MAX_TERRITORIOS * sizeof(Territorio));
    for (int i = 0; i < MAX_TERRITORIOS; i++) {
        sprintf(mapa[i].nome, "Territorio %d", i + 1);
        strcpy(mapa[i].cor, (i % 2 == 0) ? "vermelho" : "azul");
        mapa[i].tropas = 3 + rand() % 3;
    }
    strcpy(mapa[5].nome, "Amazonas"); // Território especial

    printf("=== JOGO WAR ===\n\n");

    // Exibir missão inicial para cada jogador
    for (int i = 0; i < MAX_JOGADORES; i++) {
        printf("Jogador %d - Missao: ", i + 1);
        exibirMissao(missoesJogadores[i]);
        printf("\n");
    }

    int turno = 0;
    int vencedor = -1;

    // Loop principal do jogo
    while (vencedor == -1) {
        printf("\n=== TURNO %d ===\n", turno + 1);
        exibirMapa(mapa, MAX_TERRITORIOS);

        // Jogador atual
        int jogadorAtual = turno % MAX_JOGADORES;
        const char* corAtual = (jogadorAtual == 0) ? "vermelho" : "azul";

        printf("Jogador %d (%s), ataque!\n", jogadorAtual + 1, corAtual);

        // Ataque automático simples
        int i, j;
        for (i = 0; i < MAX_TERRITORIOS; i++) {
            if (strcmp(mapa[i].cor, corAtual) == 0) {
                for (j = 0; j < MAX_TERRITORIOS; j++) {
                    if (strcmp(mapa[j].cor, corAtual) != 0) {
                        atacar(&mapa[i], &mapa[j]);
                        goto ataqueFeito;
                    }
                }
            }
        }
        ataqueFeito:

        // Verificação de missão
        if (verificarMissao(missoesJogadores[jogadorAtual], mapa, MAX_TERRITORIOS, corAtual)) {
            vencedor = jogadorAtual;
            break;
        }

        turno++;
    }

    // Vencedor encontrado
    printf("\n🎉 Jogador %d venceu! Missao cumprida: %s\n", vencedor + 1, missoesJogadores[vencedor]);

    liberarMemoria(missoesJogadores, mapa);
    return 0;
}

// ======== FUNÇÕES =========

// Exibe o mapa atual com cores e tropas
void exibirMapa(Territorio* mapa, int tamanho) {
    printf("\n--- MAPA ---\n");
    for (int i = 0; i < tamanho; i++) {
        printf("%s [%s] - Tropas: %d\n", mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Simula o ataque entre territórios
void atacar(Territorio* atacante, Territorio* defensor) {
    if (strcmp(atacante->cor, defensor->cor) == 0) return;

    int dadoAtacante = 1 + rand() % 6;
    int dadoDefensor = 1 + rand() % 6;

    printf("\nAtaque de %s (%s) para %s (%s)\n", atacante->nome, atacante->cor, defensor->nome, defensor->cor);
    printf("Dado atacante: %d | Dado defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;
        printf("Vitória! %s agora é do jogador %s.\n", defensor->nome, defensor->cor);
    } else {
        atacante->tropas--;
        printf("Derrota! Tropas restantes em %s: %d\n", atacante->nome, atacante->tropas);
    }
}

// Sorteia e atribui missão a um jogador
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strcpy(destino, missoes[indice]);
}

// Exibe a missão do jogador
void exibirMissao(const char* missao) {
    printf("%s\n", missao);
}

// Verifica se a missão foi cumprida
int verificarMissao(char* missao, Territorio* mapa, int tamanho, const char* corJogador) {
    if (strstr(missao, "Eliminar todas as tropas da cor vermelha")) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "vermelho") == 0 && strcmp(corJogador, "vermelho") != 0) return 0;
        }
        return 1;
    }

    if (strstr(missao, "Eliminar todas as tropas da cor azul")) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, "azul") == 0 && strcmp(corJogador, "azul") != 0) return 0;
        }
        return 1;
    }

    if (strstr(missao, "Conquistar 3 territorios seguidos")) {
        int count = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0) {
                count++;
                if (count >= 3) return 1;
            } else {
                count = 0;
            }
        }
    }

    if (strstr(missao, "Conquistar o territorio 'Amazonas'")) {
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].nome, "Amazonas") == 0 && strcmp(mapa[i].cor, corJogador) == 0) {
                return 1;
            }
        }
    }

    if (strstr(missao, "Controlar 2 territorios com pelo menos 5 tropas")) {
        int count = 0;
        for (int i = 0; i < tamanho; i++) {
            if (strcmp(mapa[i].cor, corJogador) == 0 && mapa[i].tropas >= 5) {
                count++;
                if (count >= 2) return 1;
            }
        }
    }

    return 0;
}

// Libera a memória alocada dinamicamente
void liberarMemoria(char** missoesJogadores, Territorio* mapa) {
    for (int i = 0; i < MAX_JOGADORES; i++) {
        free(missoesJogadores[i]);
    }
    free(mapa);
}
