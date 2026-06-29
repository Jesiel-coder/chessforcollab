#ifndef RANKING_H
#define RANKING_H

#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <string>

using namespace std;

struct RegistroRanking {
    string nome;
    int jogadas;
};

// Salva uma partida
inline void salvarRanking(string nome, int jogadas) {
    try {
        ofstream arquivo("ranking.txt", ios::app);

        if (!arquivo.is_open())
            throw runtime_error("Falha ao abrir ranking");

        arquivo << nome << ";" << jogadas << "\n";

        arquivo.close();
    }
    catch (const exception& erro) {
        cout << "\n[ERRO] " << erro.what() << endl;
    }
}

// Carrega tudo
inline vector<RegistroRanking> carregarRanking() {

    vector<RegistroRanking> ranking;

    try {

        ifstream arquivo("ranking.txt");

        if (!arquivo.is_open())
            return ranking;

        string linha;

        while (getline(arquivo, linha)) {

            size_t separador = linha.find(";");

            if (separador == string::npos)
                continue;

            RegistroRanking r;

            r.nome =
                linha.substr(
                    0,
                    separador
                );

            r.jogadas =
                stoi(
                    linha.substr(
                        separador + 1
                    )
                );

            ranking.push_back(r);
        }

        arquivo.close();

        sort(
            ranking.begin(),
            ranking.end(),

            [](RegistroRanking a,
               RegistroRanking b) {

                return a.jogadas < b.jogadas;
            }
        );
    }

    catch (...) {

        cout
        << "\nErro ao carregar ranking\n";
    }

    return ranking;
}

// Exibir ranking
inline void mostrarRanking() {

    auto ranking =
        carregarRanking();

    if (
        ranking.empty()
    ) {

        cout
        << "\nNenhuma partida registrada\n";

        return;
    }

    cout
    << "\n===== RANKING =====\n";

    int limite =
        min(
            10,
            (int)ranking.size()
        );

    for (
        int i = 0;
        i < limite;
        i++
    ) {

        cout
        << i + 1
        << " - "
        << ranking[i].nome
        << " ("
        << ranking[i].jogadas
        << " jogadas)\n";
    }

    cout
    << "===================\n";
}

#endif
