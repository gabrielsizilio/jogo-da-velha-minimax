#include <iostream>
#include <vector>
#include <limits>
#include <tuple>

#define SIZE 4

using namespace std;

enum Player
{
    O,
    X,
    VAZIO
};
using Tabuleiro = vector<vector<Player>>;
using Acao = pair<int, int>;

int maxValor(Tabuleiro tabuleiro, int depth);
int minValor(Tabuleiro tabuleiro, int depth);

Tabuleiro inicializarTabuleiro()
{
    return Tabuleiro(SIZE, vector<Player>(SIZE, VAZIO));
}

void imprimirTabuleiro(const Tabuleiro &tabuleiro)
{
    
    for (const auto &linha : tabuleiro)
    {
        for (auto celula : linha)
        {
            if (celula == X)
                cout << "X ";
            else if (celula == O)
                cout << "O ";
            else
                cout << "- ";
        }
        cout << endl;
    }
    cout << endl;
}

Player jogadorAtual(const Tabuleiro &tabuleiro)
{
    int contadorX = 0, contadorO = 0;
    for (const auto &linha : tabuleiro)
    {
        for (auto celula : linha)
        {
            if (celula == X)
                ++contadorX;
            else if (celula == O)
                ++contadorO;
        }
    }
    return (contadorX >= contadorO) ? O : X;
}

vector<Acao> acoes(const Tabuleiro &tabuleiro)
{
    vector<Acao> acoesDisponiveis;
    for (long unsigned int i = 0; i < tabuleiro.size(); ++i)
    {
        for (long unsigned int j = 0; j < tabuleiro[i].size(); ++j)
        {
            if (tabuleiro[i][j] == VAZIO)
            {
                acoesDisponiveis.emplace_back(i, j);
            }
        }
    }

    return acoesDisponiveis;
}

Tabuleiro resultado(Tabuleiro tabuleiro, Acao acao)
{
    if (tabuleiro[acao.first][acao.second] == VAZIO)
    {
        tabuleiro[acao.first][acao.second] = jogadorAtual(tabuleiro);
    }
    return tabuleiro;
}

Player ganhador(const Tabuleiro &tabuleiro)
{
    // for (long unsigned int i = 0; i < tabuleiro.size(); ++i)
    // {
    //     if (tabuleiro[i][0] != VAZIO && tabuleiro[i][0] == tabuleiro[i][1] && tabuleiro[i][1] == tabuleiro[i][2])
    //         return tabuleiro[i][0];
    //     if (tabuleiro[0][i] != VAZIO && tabuleiro[0][i] == tabuleiro[1][i] && tabuleiro[1][i] == tabuleiro[2][i])
    //         return tabuleiro[0][i];
    // }
    // if (tabuleiro[0][0] != VAZIO && tabuleiro[0][0] == tabuleiro[1][1] && tabuleiro[1][1] == tabuleiro[2][2])
    //     return tabuleiro[0][0];
    // if (tabuleiro[0][2] != VAZIO && tabuleiro[0][2] == tabuleiro[1][1] && tabuleiro[1][1] == tabuleiro[2][0])
    //     return tabuleiro[0][2];
    
    for (long unsigned int i = 0; i < tabuleiro.size(); ++i)
    {
        if (tabuleiro[i][0] != VAZIO && tabuleiro[i][0] == tabuleiro[i][1] && tabuleiro[i][1] == tabuleiro[i][2] && tabuleiro[i][2] == tabuleiro[i][3])
            return tabuleiro[i][0];
        if (tabuleiro[0][i] != VAZIO && tabuleiro[0][i] == tabuleiro[1][i] && tabuleiro[1][i] == tabuleiro[2][i] && tabuleiro[2][i] == tabuleiro[3][i])
            return tabuleiro[0][i];
    }
    if (tabuleiro[0][0] != VAZIO && tabuleiro[0][0] == tabuleiro[1][1] && tabuleiro[1][1] == tabuleiro[2][2] && tabuleiro[2][2] == tabuleiro[3][3])
        return tabuleiro[0][0];
    if (tabuleiro[0][3] != VAZIO && tabuleiro[0][3] == tabuleiro[1][2] && tabuleiro[1][2] == tabuleiro[2][1] && tabuleiro[2][1] == tabuleiro[3][0])
        return tabuleiro[0][3];
    return VAZIO;
}

bool final(const Tabuleiro &tabuleiro)
{
    if (ganhador(tabuleiro) != VAZIO)
        return true;
    for (const auto &linha : tabuleiro)
    {
        for (auto celula : linha)
        {
            if (celula == VAZIO)
                return false;
        }
    }
    return true;
}

int custo(const Tabuleiro &tabuleiro)
{
    Player vencedor = ganhador(tabuleiro);
    if (vencedor == X)
        return 1;
    if (vencedor == O)
        return -1;
    return 0;
}

int maxValor(Tabuleiro tabuleiro, int depth)
{
    depth--;
    if (final(tabuleiro) || depth == 0)
        return custo(tabuleiro);

    int valor = numeric_limits<int>::min();
    for (Acao acao : acoes(tabuleiro))
    {
        Tabuleiro novoTabuleiro = resultado(tabuleiro, acao);
        valor = max(valor, minValor(novoTabuleiro, depth));
    }
    return valor;
}

int minValor(Tabuleiro tabuleiro, int depth)
{
    depth--;
    if (final(tabuleiro) || depth == 0)
        return custo(tabuleiro);

    int valor = numeric_limits<int>::max();
    for (Acao acao : acoes(tabuleiro))
    {
        Tabuleiro novoTabuleiro = resultado(tabuleiro, acao);
        valor = min(valor, maxValor(novoTabuleiro, depth));
    }
    return valor;
}

Acao minimax(Tabuleiro tabuleiro, int depth)
{
    int melhorValor = numeric_limits<int>::min();
    Acao melhorAcao = {-1, -1};
    for (Acao acao : acoes(tabuleiro))
    {
        Tabuleiro novoTabuleiro = resultado(tabuleiro, acao);
        int valorAtual = (jogadorAtual(tabuleiro) == X) ? minValor(novoTabuleiro, depth) : maxValor(novoTabuleiro, depth);
        if (valorAtual > melhorValor)
        {
            melhorValor = valorAtual;
            melhorAcao = acao;
        }
    }
    return melhorAcao;
}

int main()
{
    Tabuleiro tabuleiro = inicializarTabuleiro();
    
    int depth = 0;
    cout << "[Profundidade]: ";
    cin >> depth;
    
    while (depth <= 0 || depth >= 7)
    {
        cout << "Escolha profundidade entre 1 e 6:";
        cin >> depth;
    }
    
    imprimirTabuleiro(tabuleiro);

    while (!final(tabuleiro))
    {
        if (jogadorAtual(tabuleiro) != O)
        {
            cout << "Vez da máquina (X):\n";
            Acao acao = minimax(tabuleiro, depth);
            tabuleiro = resultado(tabuleiro, acao);
        }
        else
        {
            int i, j;
            cout << "Sua vez (O): ";
            cin >> i >> j;
            while (i < 0 || i > SIZE || j < 0 || j > SIZE || tabuleiro[i][j] != VAZIO)
            {
                cout << "Jogada inválida. Por favor, escolha novamente: ";
                cin >> i >> j;
            }
            tabuleiro[i][j] = O;
        }
        imprimirTabuleiro(tabuleiro);

        Player vencedor = ganhador(tabuleiro);
        if (vencedor != VAZIO || final(tabuleiro))
        {
            if (vencedor == X)
                cout << "A máquina (X) venceu!\n";
            else if (vencedor == O)
                cout << "Parabéns, você venceu!\n";
            else
                cout << "Empate!\n";
            break;
        }
    }

    return 0;
}