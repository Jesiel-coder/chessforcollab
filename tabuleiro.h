#ifndef TABULEIRO_H
#define TABULEIRO_H

#include <vector>
#include <string>

using namespace std;

// Constante global para as dimensões do tabuleiro (8x8)
const int TAMANHO = 8;

// Estrutura exata da Peça (com a cor do texto inclusa)
struct Peca {
    string simbolo; // Ex: "♜", "♞" ou ""
    int corTexto;   // Ex: 91 para Pretas, 94 para Brancas, 30 para vazio
    char time;      // 'B' para Brancas, 'P' para Pretas, ou ' '
};

// Estrutura exata do Estado de Jogo (com Roque e En Passant)
struct EstadoJogo {
    char turno = 'B';
    int enPassantLinha = -1;
    int enPassantCol = -1;
    bool roqueB_curto = true;
    bool roqueB_longo = true;
    bool roqueP_curto = true;
    bool roqueP_longo = true;
};

// --- DECLARAÇÃO DAS FUNÇÕES DO TABULEIRO ---

void inicializarTabuleiro(vector<vector<Peca>>& tabuleiro);

// Exibir tabuleiro recebendo a matriz de bools como dicas (com valor padrão vazio caso não seja passado)
void exibirTabuleiro(const vector<vector<Peca>>& tabuleiro, char turno, vector<vector<bool>> dicas = {});

bool estaEmXeque(const vector<vector<Peca>>& tabuleiro, char turno);

bool temMovimentoLegal(const vector<vector<Peca>>& tabuleiro, const EstadoJogo& estado);

// Corrigido para retornar a matriz de booleans (vector<vector<bool>>) que o seu código usa
vector<vector<bool>> gerarDicas(const vector<vector<Peca>>& tabuleiro, int lO, int cO, const EstadoJogo& estado);

bool movimentoValido(const vector<vector<Peca>>& tabuleiro, int lO, int cO, int lD, int cD, char turno, const EstadoJogo& estado);

void executarMovimento(vector<vector<Peca>>& tabuleiro, int lO, int cO, int lD, int cD, EstadoJogo& estado);

#endif // TABULEIRO_H
