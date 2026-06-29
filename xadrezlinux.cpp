#include <iostream>
#include <clocale>
#include <string>
#include <limits>

#include "tabuleiro.h"
#include "Ranking.h"
#include "menu.h" // Incluindo o novo cabeçalho modularizado

using namespace std;

void jogarPartida() {
    vector<vector<Peca>> tabuleiro(
        TAMANHO,
        vector<Peca>(TAMANHO)
    );

    inicializarTabuleiro(tabuleiro);

    EstadoJogo estado;
    int numeroJogadas = 0;

    while (true) {
        exibirTabuleiro(tabuleiro, estado.turno);

        string nomeTurno = (estado.turno == 'B') ? "BRANCAS" : "PRETAS";
        bool emXeque = estaEmXeque(tabuleiro, estado.turno);
        bool temLegal = temMovimentoLegal(tabuleiro, estado);

        if (!temLegal) {
            if (emXeque) {
                cout << "\033[91m XEQUE-MATE! " << (estado.turno == 'B' ? "Pretas vencem!" : "Brancas vencem!") << "\033[0m\n";
                string nome;
                cout << "\nNome do vencedor: ";
                cin >> nome;
                salvarRanking(nome, numeroJogadas);
                mostrarRanking();
            } else {
                cout << "\033[93m AFOGAMENTO! Empate.\033[0m\n";
            }
            break;
        }

        if (emXeque) {
            cout << "\033[91m REI DAS " << nomeTurno << " EM XEQUE!\033[0m\n";
        }

        cout << "Turno das " << nomeTurno << "\n";
        string origem;
        cout << "Origem (ex: a2) ou '0' para voltar ao menu: ";
        cin >> origem;

        if (origem == "0") break;

        if (origem.length() != 2 || origem[0] < 'a' || origem[0] > 'h' || origem[1] < '1' || origem[1] > '8') {
            cout << "\033[91mErro: Formato invalido!\033[0m\n";
            continue;
        }

        int cO = origem[0] - 'a';
        int lO = TAMANHO - (origem[1] - '0');

        auto dicas = gerarDicas(tabuleiro, lO, cO, estado);
        exibirTabuleiro(tabuleiro, estado.turno, dicas);

        if (tabuleiro[lO][cO].simbolo.empty()) {
            cout << "\033[91mErro: Nao ha peca nessa posicao!\033[0m\n";
            continue;
        }

        if (tabuleiro[lO][cO].time != estado.turno) {
            cout << "\033[91mErro: Essa peca nao e sua!\033[0m\n";
            continue;
        }

        string destino;
        cout << "Destino (ex: a4): ";
        cin >> destino;

        if (destino.length() != 2 || destino[0] < 'a' || destino[0] > 'h' || destino[1] < '1' || destino[1] > '8') {
            cout << "\033[91mErro: Formato invalido!\033[0m\n";
            continue;
        }

        int cD = destino[0] - 'a';
        int lD = TAMANHO - (destino[1] - '0');

        if (!movimentoValido(tabuleiro, lO, cO, lD, cD, estado.turno, estado)) {
            cout << "\033[91mErro: Movimento invalido!\033[0m\n";
            continue;
        }

        executarMovimento(tabuleiro, lO, cO, lD, cD, estado);
        numeroJogadas++;
    }
}

int main() {
    setlocale(LC_ALL, "");

    bool rodando = true;
    while (rodando) {
        int opcao = Menu(); // Chama a função modularizada

        switch (opcao) {
            case NOVO_JOGO: // Uso do enum semântico
                jogarPartida();
                break;
            case VER_RANKING:
                mostrarRanking();
                break;
            case SAIR:
                cout << "\n\033[0mObrigado por jogar! Saindo do programa...\n";
                rodando = false;
                break;
        }
    }

    return 0;
}
