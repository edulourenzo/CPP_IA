#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>

//X[][0] = Virus
//X[][1] = Bacteria
//X[][2] = Dor de cabeca
//X[][3] = Corisa
//X[][4] = Bias
//X[][5] = [(0, Gripe), (1, Resfriado); D = Valor esperado]
bool X[6][6] =
{
    1,1,0,1,1,0,
    1,0,1,0,1,1,
    1,1,0,1,0,0,
    1,0,1,1,1,1,
    1,0,0,1,1,0,
    1,0,0,0,1,1
};

int W[6] =
{0,0,0,0,0,0,};

unsigned short ordem[6] =
{0,1,2,3,4,5};

using namespace std;

void    embaralha   (unsigned short*);
int     net         (bool*, int*);
bool    limiar      (int, double);
bool    f_ativa     (bool, bool);
void    delta       (bool[6][6], int*, double, int, int);

int main(void)
{
    bool falha;
    unsigned short cont;
    double alfa, teta;

    srand((unsigned)time(NULL));

    cout << "[0,0 - 1,0] Digite o valor de Alfa: ";
    cin >> alfa;
    cout << "Digite o valor de Teta: ";
    cin >> teta;

    embaralha(ordem);

    cout << "ordem: ";
    for(int j = 0; j < 6; j++)
        cout << ordem[j] << " ";
    cout << endl;

    cout << "Wn: ";
    for(int j = 0; j < 6; j++)
        cout << W[j] << " ";
    cout << endl;


    do
    {
        cont++;
        falha = false;
        for(int i = 0; i < 6; i++)
        {
            bool aux =
                f_ativa(X[ordem[i]][0],
                        limiar(
                            net(&X[ordem[i]][0], W), teta));

            if(aux != X[ordem[i]][5])
            {
                int temp = X[ordem[i]][5] - aux;
                falha = true;
                delta(X, W, alfa, temp, ordem[i]);
            }
        }

        cout << "Wn: ";
        for(int j = 0; j < 6; j++)
            cout << W[j] << " ";
        cout << endl;
        cout << falha << ": ";
        cin >> falha;
    }
    while(falha == true);

    cout << "Numero de interacao: " << cont << endl;

    return 0;
}

void embaralha(unsigned short* vet)
{
    unsigned short aux, i, j;

    for(int k = 0; k < 12; k++)
    {
        i = rand() % 6;
        j = rand() % 6;
        aux = vet[i];
        vet[i] = vet[j];
        vet[j] = aux;
    }
}

int net(bool* X, int* W)
{
    int u = 0;

    for(int i = 0; i < 6; i++)
    {
        u += *(X+i) * W[i];
    }

    return u;
}

bool limiar(int u, double teta)
{
    if(u >= teta) return true;

    else return false;
}

bool f_ativa(bool X, bool y)
{
    if(X == y) return true;

    else return false;
}

void delta(bool X[6][6], int* W, double alfa, int erro, int L)
{
    for(int i = 0; i < 6; i++)
        W[i] += (alfa * X[L][i] * erro);
}


