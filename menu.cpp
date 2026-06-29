#include "menu.h"
#include <iostream>
#include <limits>

using namespace std;

int Menu() {
    int escolha = 0;
    while (true) {
        cout << "=========================================\n";
        cout << "             XADREZ EM C++               \n";
        cout << "=========================================\n";
        cout << " [1] Iniciar Novo Jogo\n";
        cout << " [2] Ver Ranking\n";
        cout << " [3] Sair do Programa\n";
        cout << "-----------------------------------------\n";
        cout << "Escolha uma opcao: ";

        cin >> escolha;

        // Validação caso digitem letras ou opções inexistentes
        if (cin.fail() || escolha < NOVO_JOGO || escolha > SAIR) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "\nOpcao invalida! Tente novamente.\n\n";
        } else {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return escolha;
        }
    }
}
