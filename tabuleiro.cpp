
#include "tabuleiro.h"
#include <cstdlib>
#include <iostream>
using namespace std;

// ── Exibição ──────────────────────────────────────────────────────────────────

void aplicarCor(int corTexto, int corFundo) {
    cout << "\033[" << corTexto << ";" << corFundo << "m";
}
vector<vector<bool>> gerarDicas(
    const vector<vector<Peca>>& t,
    int lO,
    int cO,
    const EstadoJogo& estado
){

    vector<vector<bool>> dicas(
        TAMANHO,
        vector<bool>(
            TAMANHO,
            false
        )
    );

    for (
        int l=0;
        l<TAMANHO;
        l++
    ){

        for (
            int c=0;
            c<TAMANHO;
            c++
        ){

            if(
                movimentoValido(
                    t,
                    lO,
                    cO,
                    l,
                    c,
                    estado.turno,
                    estado
                )
            ){

                dicas[l][c]=true;
            }
        }
    }

    return dicas;
}

void inicializarTabuleiro(vector<vector<Peca>>& t) {
    string pecasP[] = {"♜","♞","♝","♛","♚","♝","♞","♜"};
    string pecasB[] = {"♖","♘","♗","♕","♔","♗","♘","♖"};
    for (int j = 0; j < TAMANHO; j++) {
        t[0][j] = {pecasP[j], 91, 'P'};
        t[1][j] = {"♟",       91, 'P'};
    }
    for (int i = 2; i < 6; i++)
        for (int j = 0; j < TAMANHO; j++)
            t[i][j] = {"", 30, ' '};
    for (int j = 0; j < TAMANHO; j++) {
        t[6][j] = {"♙",       94, 'B'};
        t[7][j] = {pecasB[j], 94, 'B'};
    }
}

void exibirTabuleiro(const vector<vector<Peca>>& t, char turno, vector<vector<bool>> dicas) {
    int fundoClaro=107, fundoEscuro=43, fundoBorda=40, textoBorda=37;
    bool inv = (turno == 'P');

    aplicarCor(textoBorda, fundoBorda);
    cout << "     "; //inicio da borda das letras
    for (int j = 0; j < TAMANHO; j++) {
        int col = inv ? (TAMANHO-1-j) : j;
        cout << (char)('a'+col) << "   "; // aumentar o tamanho da boorda das letras
    }
    cout << "   " << endl;

    for (int i = 0; i < TAMANHO; i++) {
        int linha = inv ? (TAMANHO-1-i) : i;
        aplicarCor(textoBorda, fundoBorda);
        cout << " " << (inv ? (i+1) : (TAMANHO-i)) << " ";
        for (int j = 0; j < TAMANHO; j++) {
            int col = inv ? (TAMANHO-1-j) : j;
            int fundo = ((linha+col)%2==0) ? fundoClaro : fundoEscuro;
            aplicarCor(t[linha][col].corTexto, fundo);
            const string& s = t[linha][col].simbolo;
            if (s.empty() && !dicas.empty() && dicas[linha][col]) {
            cout << " • ";
} else if( s.empty()) {
    cout << "    "; //casas vazias
} else{
    cout << "  " << s; //casas preenchidas
}
}
        aplicarCor(textoBorda, fundoBorda);
        cout << "   " << endl;
    }
    cout << "\033[0m" << endl;
}

// ── Helpers ───────────────────────────────────────────────────────────────────

static bool caminhoLivre(const vector<vector<Peca>>& t,
                         int lO, int cO, int lD, int cD) {
    int dl=(lD>lO)?1:(lD<lO)?-1:0;
    int dc=(cD>cO)?1:(cD<cO)?-1:0;
    int l=lO+dl, c=cO+dc;
    while (l!=lD || c!=cD) {
        if (!t[l][c].simbolo.empty()) return false;
        l+=dl; c+=dc;
    }
    return true;
}

// Verifica se uma casa está atacada pelo inimigo (usado no roque)
static bool casaAtacada(const vector<vector<Peca>>& t, int l, int c, char turno) {
    char inimigo = (turno=='B') ? 'P' : 'B';
    for (int i=0; i<TAMANHO; i++)
        for (int j=0; j<TAMANHO; j++)
            if (t[i][j].time == inimigo) {
                // Checa mecanica basica sem roque pra evitar recursao
                int dl=l-i, dc=c-j, adl=abs(dl), adc=abs(dc);
                const string& s = t[i][j].simbolo;
                if (s=="♙"||s=="♟") {
                    int dir=(inimigo=='B')?-1:1;
                    if (dl==dir && adc==1) return true;
                } else if (s=="♖"||s=="♜") {
                    if ((dl==0||dc==0) && caminhoLivre(t,i,j,l,c)) return true;
                } else if (s=="♗"||s=="♝") {
                    if (adl==adc && caminhoLivre(t,i,j,l,c)) return true;
                } else if (s=="♕"||s=="♛") {
                    if ((dl==0||dc==0||adl==adc) && caminhoLivre(t,i,j,l,c)) return true;
                } else if (s=="♔"||s=="♚") {
                    if (adl<=1 && adc<=1) return true;
                } else if (s=="♘"||s=="♞") {
                    if ((adl==2&&adc==1)||(adl==1&&adc==2)) return true;
                }
            }
    return false;
}

// ── Mecânica ──────────────────────────────────────────────────────────────────

bool mecanicaValida(const vector<vector<Peca>>& t,
                    int lO, int cO, int lD, int cD, char turno,
                    const EstadoJogo& estado) {
    if (lO==lD && cO==cD) return false;
    const Peca& o = t[lO][cO];
    const Peca& d = t[lD][cD];
    if (o.time != turno) return false;
    if (d.time == turno) return false;

    int dl=lD-lO, dc=cD-cO, adl=abs(dl), adc=abs(dc);
    const string& s = o.simbolo;

    // ── Peão ──
    if (s=="♙"||s=="♟") {
        int dir=(turno=='B')?-1:1, ini=(turno=='B')?6:1;
        if (dc==0) {
            if (dl==dir && d.simbolo.empty()) return true;
            if (dl==2*dir && lO==ini && d.simbolo.empty()
                && t[lO+dir][cO].simbolo.empty()) return true;
        }
        // Captura normal
        if (dl==dir && adc==1 && !d.simbolo.empty()) return true;
        // En passant
        if (dl==dir && adc==1
            && lD==estado.enPassantLinha && cD==estado.enPassantCol)
            return true;
        return false;
    }

    // ── Torre ──
    if (s=="♖"||s=="♜")
        return (dl==0||dc==0) && caminhoLivre(t,lO,cO,lD,cD);

    // ── Bispo ──
    if (s=="♗"||s=="♝")
        return (adl==adc) && caminhoLivre(t,lO,cO,lD,cD);

    // ── Rainha ──
    if (s=="♕"||s=="♛")
        return (dl==0||dc==0||adl==adc) && caminhoLivre(t,lO,cO,lD,cD);

    // ── Rei ──
    if (s=="♔"||s=="♚") {
        if (adl<=1 && adc<=1) return true;

        // Roque curto (lado do rei, coluna 6)
        if (dl==0 && dc==2) {
            bool direito = (turno=='B') ? estado.roqueB_curto : estado.roqueP_curto;
            int lin = (turno=='B') ? 7 : 0;
            if (!direito || lO!=lin || cO!=4) return false;
            // Caminho livre e não passa por xeque
            if (!t[lin][5].simbolo.empty() || !t[lin][6].simbolo.empty()) return false;
            if (casaAtacada(t,lin,4,turno)) return false;
            if (casaAtacada(t,lin,5,turno)) return false;
            if (casaAtacada(t,lin,6,turno)) return false;
            return true;
        }

        // Roque longo (lado da rainha, coluna 2)
        if (dl==0 && dc==-2) {
            bool direito = (turno=='B') ? estado.roqueB_longo : estado.roqueP_longo;
            int lin = (turno=='B') ? 7 : 0;
            if (!direito || lO!=lin || cO!=4) return false;
            if (!t[lin][3].simbolo.empty() || !t[lin][2].simbolo.empty()
                || !t[lin][1].simbolo.empty()) return false;
            if (casaAtacada(t,lin,4,turno)) return false;
            if (casaAtacada(t,lin,3,turno)) return false;
            if (casaAtacada(t,lin,2,turno)) return false;
            return true;
        }

        return false;
    }

    // ── Cavalo ──
    if (s=="♘"||s=="♞")
        return (adl==2&&adc==1)||(adl==1&&adc==2);

    return false;
}

// ── Xeque ─────────────────────────────────────────────────────────────────────

bool estaEmXeque(const vector<vector<Peca>>& t, char turno) {
    string rei = (turno=='B') ? "♔" : "♚";
    int lRei=-1, cRei=-1;
    for (int i=0; i<TAMANHO && lRei==-1; i++)
        for (int j=0; j<TAMANHO && lRei==-1; j++)
            if (t[i][j].simbolo==rei) { lRei=i; cRei=j; }
    if (lRei==-1) return false;
    return casaAtacada(t, lRei, cRei, turno);
}

bool movimentoValido(const vector<vector<Peca>>& t,
                     int lO, int cO, int lD, int cD, char turno,
                     const EstadoJogo& estado) {
    if (!mecanicaValida(t,lO,cO,lD,cD,turno,estado)) return false;
    vector<vector<Peca>> copia = t;
    copia[lD][cD] = copia[lO][cO];
    copia[lO][cO] = {"",30,' '};
    // Remove peão capturado en passant
    if ((t[lO][cO].simbolo=="♙"||t[lO][cO].simbolo=="♟")
        && cD==estado.enPassantCol && lD==estado.enPassantLinha)
        copia[lO][cD] = {"",30,' '};
    return !estaEmXeque(copia, turno);
}

bool temMovimentoLegal(const vector<vector<Peca>>& t, const EstadoJogo& estado) {
    for (int lO=0; lO<TAMANHO; lO++)
        for (int cO=0; cO<TAMANHO; cO++)
            if (t[lO][cO].time==estado.turno)
                for (int lD=0; lD<TAMANHO; lD++)
                    for (int cD=0; cD<TAMANHO; cD++)
                        if (movimentoValido(t,lO,cO,lD,cD,estado.turno,estado))
                            return true;
    return false;
}

// ── Executa movimento + atualiza estado ───────────────────────────────────────

void executarMovimento(vector<vector<Peca>>& t,
                       int lO, int cO, int lD, int cD,
                       EstadoJogo& estado) {
    const string s = t[lO][cO].simbolo;
    char turno = estado.turno;

    // Reseta en passant (vale só por 1 turno)
    estado.enPassantLinha = -1;
    estado.enPassantCol   = -1;

    // En passant: remove o peão capturado
    if ((s=="♙"||s=="♟") && cD!=cO && t[lD][cD].simbolo.empty()) {
        t[lO][cD] = {"",30,' '};
    }

    // Marca casa de en passant se peão avançou 2
    if ((s=="♙"||s=="♟") && abs(lD-lO)==2) {
        estado.enPassantLinha = (lO+lD)/2;
        estado.enPassantCol   = cO;
    }

    // Roque: move a torre junto
    if (s=="♔"||s=="♚") {
        int lin = (turno=='B') ? 7 : 0;
        if (cD==6) { // curto
            t[lin][5] = t[lin][7];
            t[lin][7] = {"",30,' '};
        } else if (cD==2) { // longo
            t[lin][3] = t[lin][0];
            t[lin][0] = {"",30,' '};
        }
        // Perde direito ao roque
        if (turno=='B') { estado.roqueB_curto=false; estado.roqueB_longo=false; }
        else             { estado.roqueP_curto=false; estado.roqueP_longo=false; }
    }

    // Atualiza direito ao roque se torre se mover
    if (s=="♖") {
        if (lO==7 && cO==7) estado.roqueB_curto = false;
        if (lO==7 && cO==0) estado.roqueB_longo = false;
    }
    if (s=="♜") {
        if (lO==0 && cO==7) estado.roqueP_curto = false;
        if (lO==0 && cO==0) estado.roqueP_longo = false;
    }

    // Executa o movimento
    t[lD][cD] = t[lO][cO];
    t[lO][cO] = {"",30,' '};

    // Promoção do peão
    if (s=="♙" && lD==0) {
        cout << "\033[93mPromocao! Escolha: (D)ama (T)orre (B)ispo (C)avalo: \033[0m";
        char escolha; cin >> escolha;
        switch(toupper(escolha)) {
            case 'T': t[lD][cD] = {"♖",94,'B'}; break;
            case 'B': t[lD][cD] = {"♗",94,'B'}; break;
            case 'C': t[lD][cD] = {"♘",94,'B'}; break;
            default:  t[lD][cD] = {"♕",94,'B'}; break;
        }
    }
    if (s=="♟" && lD==7) {
        cout << "\033[93mPromocao! Escolha: (D)ama (T)orre (B)ispo (C)avalo: \033[0m";
        char escolha; cin >> escolha;
        switch(toupper(escolha)) {
            case 'T': t[lD][cD] = {"♜",91,'P'}; break;
            case 'B': t[lD][cD] = {"♝",91,'P'}; break;
            case 'C': t[lD][cD] = {"♞",91,'P'}; break;
            default:  t[lD][cD] = {"♛",91,'P'}; break;
        }
    }

    // Troca turno
    estado.turno = (turno=='B') ? 'P' : 'B';
}
