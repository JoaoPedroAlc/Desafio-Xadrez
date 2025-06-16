
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Definir as peças do xadrez
typedef enum {
    VAZIO = 0,
    PEAO_BRANCO = 1, TORRE_BRANCA = 2, CAVALO_BRANCO = 3, BISPO_BRANCO = 4, RAINHA_BRANCA = 5, REI_BRANCO = 6,
    PEAO_PRETO = -1, TORRE_PRETA = -2, CAVALO_PRETO = -3, BISPO_PRETO = -4, RAINHA_PRETA = -5, REI_PRETO = -6
} Peca;

// Tabuleiro 8x8
Peca tabuleiro[8][8];
int vez_brancas = 1; // 1 para brancas, 0 para pretas

// Função para inicializar o tabuleiro
void inicializar_tabuleiro() {
    // Limpar tabuleiro
    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 8; j++) {
            tabuleiro[i][j] = VAZIO;
        }
    }
    
    // Peças pretas
    tabuleiro[0][0] = TORRE_PRETA; tabuleiro[0][7] = TORRE_PRETA;
    tabuleiro[0][1] = CAVALO_PRETO; tabuleiro[0][6] = CAVALO_PRETO;
    tabuleiro[0][2] = BISPO_PRETO; tabuleiro[0][5] = BISPO_PRETO;
    tabuleiro[0][3] = RAINHA_PRETA;
    tabuleiro[0][4] = REI_PRETO;
    
    for (int j = 0; j < 8; j++) {
        tabuleiro[1][j] = PEAO_PRETO;
    }
    
    // Peças brancas
    tabuleiro[7][0] = TORRE_BRANCA; tabuleiro[7][7] = TORRE_BRANCA;
    tabuleiro[7][1] = CAVALO_BRANCO; tabuleiro[7][6] = CAVALO_BRANCO;
    tabuleiro[7][2] = BISPO_BRANCO; tabuleiro[7][5] = BISPO_BRANCO;
    tabuleiro[7][3] = RAINHA_BRANCA;
    tabuleiro[7][4] = REI_BRANCO;
    
    for (int j = 0; j < 8; j++) {
        tabuleiro[6][j] = PEAO_BRANCO;
    }
}

// Função para obter o símbolo da peça
char obter_simbolo_peca(Peca peca) {
    switch (peca) {
        case PEAO_BRANCO: return 'P';
        case TORRE_BRANCA: return 'T';
        case CAVALO_BRANCO: return 'C';
        case BISPO_BRANCO: return 'B';
        case RAINHA_BRANCA: return 'D';
        case REI_BRANCO: return 'R';
        case PEAO_PRETO: return 'p';
        case TORRE_PRETA: return 't';
        case CAVALO_PRETO: return 'c';
        case BISPO_PRETO: return 'b';
        case RAINHA_PRETA: return 'd';
        case REI_PRETO: return 'r';
        default: return '.';
    }
}

// Função para mostrar o tabuleiro
void mostrar_tabuleiro() {
    printf("\n    a b c d e f g h\n");
    printf("  +----------------+\n");
    
    for (int i = 0; i < 8; i++) {
        printf("%d | ", 8 - i);
        for (int j = 0; j < 8; j++) {
            printf("%c ", obter_simbolo_peca(tabuleiro[i][j]));
        }
        printf("| %d\n", 8 - i);
    }
    
    printf("  +----------------+\n");
    printf("    a b c d e f g h\n\n");
    
    printf("Legenda: P=Peão, T=Torre, C=Cavalo, B=Bispo, D=Dama/Rainha, R=Rei\n");
    printf("Maiúscula=Brancas, Minúscula=Pretas\n\n");
}

// Função para converter notação (ex: "a1") para coordenadas
int converter_posicao(char* pos, int* linha, int* coluna) {
    if (strlen(pos) != 2) return 0;
    
    char col_char = tolower(pos[0]);
    char lin_char = pos[1];
    
    if (col_char < 'a' || col_char > 'h') return 0;
    if (lin_char < '1' || lin_char > '8') return 0;
    
    *coluna = col_char - 'a';
    *linha = 8 - (lin_char - '0');
    
    return 1;
}

// Função para verificar se a peça pertence ao jogador atual
int peca_do_jogador(Peca peca) {
    if (vez_brancas && peca > 0) return 1;
    if (!vez_brancas && peca < 0) return 1;
    return 0;
}

// Função básica para verificar movimento válido (simplificada)
int movimento_valido(int origem_linha, int origem_coluna, int destino_linha, int destino_coluna) {
    // Verificar se as posições estão dentro do tabuleiro
    if (origem_linha < 0 || origem_linha >= 8 || origem_coluna < 0 || origem_coluna >= 8) return 0;
    if (destino_linha < 0 || destino_linha >= 8 || destino_coluna < 0 || destino_coluna >= 8) return 0;
    
    // Verificar se há uma peça na origem
    Peca peca_origem = tabuleiro[origem_linha][origem_coluna];
    if (peca_origem == VAZIO) return 0;
    
    // Verificar se a peça pertence ao jogador atual
    if (!peca_do_jogador(peca_origem)) return 0;
    
    // Verificar se o destino não tem uma peça da mesma cor
    Peca peca_destino = tabuleiro[destino_linha][destino_coluna];
    if (peca_destino != VAZIO && peca_do_jogador(peca_destino)) return 0;
    
    // Movimento básico do peão (simplificado)
    if (abs(peca_origem) == 1) { // Peão
        int direcao = (peca_origem > 0) ? -1 : 1;
        
        // Movimento para frente
        if (origem_coluna == destino_coluna) {
            if (destino_linha == origem_linha + direcao && peca_destino == VAZIO) return 1;
            // Movimento duplo inicial
            if ((origem_linha == 6 && peca_origem > 0) || (origem_linha == 1 && peca_origem < 0)) {
                if (destino_linha == origem_linha + 2 * direcao && peca_destino == VAZIO) return 1;
            }
        }
        // Captura diagonal
        else if (abs(origem_coluna - destino_coluna) == 1 && destino_linha == origem_linha + direcao) {
            if (peca_destino != VAZIO && !peca_do_jogador(peca_destino)) return 1;
        }
        return 0;
    }
    
    // Para outras peças, aceitar qualquer movimento (simplificado)
    return 1;
}

// Função para fazer um movimento
int fazer_movimento(char* origem, char* destino) {
    int origem_linha, origem_coluna, destino_linha, destino_coluna;
    
    if (!converter_posicao(origem, &origem_linha, &origem_coluna)) {
        printf("Posição de origem inválida!\n");
        return 0;
    }
    
    if (!converter_posicao(destino, &destino_linha, &destino_coluna)) {
        printf("Posição de destino inválida!\n");
        return 0;
    }
    
    if (!movimento_valido(origem_linha, origem_coluna, destino_linha, destino_coluna)) {
        printf("Movimento inválido!\n");
        return 0;
    }
    
    // Fazer o movimento
    tabuleiro[destino_linha][destino_coluna] = tabuleiro[origem_linha][origem_coluna];
    tabuleiro[origem_linha][origem_coluna] = VAZIO;
    
    // Trocar vez
    vez_brancas = !vez_brancas;
    
    return 1;
}

// Função principal do jogo
int main() {
    char origem[10], destino[10];
    char comando[20];
    
    printf("=== JOGO DE XADREZ ===\n");
    printf("Digite os movimentos no formato: e2 e4\n");
    printf("Digite 'sair' para terminar o jogo\n");
    printf("Digite 'ajuda' para ver os comandos\n\n");
    
    inicializar_tabuleiro();
    
    while (1) {
        mostrar_tabuleiro();
        
        printf("Vez das %s\n", vez_brancas ? "BRANCAS" : "PRETAS");
        printf("Digite seu movimento (origem destino): ");
        
        if (scanf("%s", comando) != 1) {
            printf("Erro na entrada!\n");
            continue;
        }
        
        if (strcmp(comando, "sair") == 0) {
            printf("Obrigado por jogar!\n");
            break;
        }
        
        if (strcmp(comando, "ajuda") == 0) {
            printf("\nCOMANDOS:\n");
            printf("- Digite dois locais separados por espaço (ex: e2 e4)\n");
            printf("- Colunas: a-h, Linhas: 1-8\n");
            printf("- 'sair' para terminar\n");
            printf("- 'ajuda' para ver esta mensagem\n\n");
            continue;
        }
        
        // Ler o destino
        if (scanf("%s", destino) != 1) {
            printf("Erro na entrada do destino!\n");
            continue;
        }
        
        strcpy(origem, comando);
        
        if (fazer_movimento(origem, destino)) {
            printf("Movimento realizado: %s -> %s\n\n", origem, destino);
        } else {
            printf("Tente novamente!\n\n");
        }
    }
    
    return 0;
}
