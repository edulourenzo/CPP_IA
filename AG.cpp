#include <iostream>     //cin, cout, fixed, precision
#include <vector>       //vector
#include <cmath>        //pow
#include <algorithm>    //max, min, max_element, min_element
#include <ctime>        //time
#include <cstdlib>      //srand, rand

#define     MIN     -100
#define     MAX     100
#define     P       5
#define     K       22
#define     N       2*K

using namespace std;

void    inicializa  (vector<vector<bool> >&, int);
int     parte_x     (vector<bool>&);
int     parte_y     (vector<bool>&);
double  int_real    (int);
double  F6          (double, double);
void    avalia      (vector<vector<bool> >&, int, vector<double>&);
void    somatorio   (vector<double>&, vector<int>&, int);
int     roleta      (vector<int>&, int);
void    crossover   (vector<vector<bool> >&, vector<vector<bool> >&,
                     vector<int>&, int, double);
void    multacacao  (vector<vector<bool> >&, int, double);

int main(void)
{
    int num_pop, num_ger, total_ind, t;
    double taxa_mut, taxa_cro, max_ft, min_ft, aux_ft;

    //cria semente para numeros aleatorios
    srand((unsigned)time(NULL));

    //numero de casas decimais a ser impresso
    cout.precision(P);

    //entrada dos parametros
    cout << "Numero da Populacao: ";
    cin >> num_pop;

    cout << "Numero de Geracoes: ";
    cin >> num_ger;

    cout << "[0,6 - 0,9] Taxa de Crossover: ";
    cin >> taxa_cro;

    cout << "[0,001 - 0,05] Taxa de Multacao: ";
    cin >> taxa_mut;

    total_ind = num_ger * num_pop;
    cout << "Total de Individuos: " << total_ind << endl;

    //cria populacao e aptidao
    vector<vector<bool> > pais(num_pop, vector<bool>(N));
    vector<vector<bool> > filhos(num_pop, vector<bool>(N));
    vector<double> aptidao(num_pop);
    vector<int> soma(num_pop);

    //inicializa populacao aleatoriamente
    inicializa(pais, num_pop);

    //avalia a aptidao da primeira geracao
    avalia(pais, num_pop, aptidao);

    //inicializa variaveis de impressao
    max_ft = 0.0;
    min_ft = 1.0;
    t = 0;
    do
    {
        cout << endl << "Numero da geracao: " << t+1;

        //preenche vetor auxiliar para roleta
        somatorio(aptidao, soma, num_pop);

        //cruza a geracao dos pais para criar a geracao dos filhos
        crossover(pais, filhos, soma, num_pop, taxa_cro);

        //multa a geracao dos filhos
        multacacao(filhos, num_pop, taxa_mut);

        pais = filhos;

        //avalia a aptidao da nova geracao
        avalia(pais, num_pop, aptidao);

        //guarda valor maximo
        aux_ft = *max_element(aptidao.begin(), aptidao.end());
        cout << "\tMaior aptidao: " << aux_ft;
        if(aux_ft > max_ft)
            max_ft = aux_ft;

        //guarda valor minimo
        aux_ft = *min_element(aptidao.begin(), aptidao.end());
        cout << "\tMenor  aptidao: " << aux_ft << endl;
        if(aux_ft < min_ft)
            min_ft = aux_ft;

        t++;
    }
    while(t < num_ger);

    //imprime valores maximo e minimo durante execucao
    cout.precision(N);
    cout << endl << "Valor Maximo: " << max_ft
         << "\nValor Minimo: "<< min_ft << endl;

    return 0;
}

void inicializa(vector<vector<bool> > &p, int np)
{
    for(int i = 0; i < np; i++)
    {
        for(int j = 0; j < N; j++)
            p[i][j] = rand()%2;
    }
}

int parte_x(vector<bool> &ind)
{
    int x = 0;
    for(int i = 0; i < K; i++ )
        x = x | (ind[i] << ((K - 1) - i));

    return x;

}

int parte_y(vector<bool>& ind)
{
    int y = 0;

    for(int i = K; i < N; i++)
        y = y | (ind[i] << ((N-1) - i));

    return y;
}

double int_real(int z)
{
    return ((z * (MAX - MIN) / (pow(2, K) - 1)) + MIN);
}

double F6(double x, double y)
{
    double aux = x * x + y * y;

    return 0.5 - ((pow(sin(sqrt(aux)),2) - 0.5) / (pow(1.0 + 0.001 * aux, 2)));
}

void avalia(vector<vector<bool> >& p, int np, vector<double> &ft)
{
    for(int i = 0; i < np; i++)
        ft[i] = F6(int_real(parte_x(p[i])), int_real(parte_y(p[i])));
}

void somatorio(vector<double>& ft, vector<int>& soma, int np)
{
    soma[0] = (int)(ft[0]*10);
    for(int i = 1; i < np; i++)
        soma[i] = soma[i-1] + (int)(ft[i]*10);
}

int roleta(vector<int>& soma, int np)
{
    int i, j;

    i = 0;
    j = rand() % soma[np-1];

    while(soma[i] < j && i < np)
        i++;

    return i;
}

void crossover(vector<vector<bool> >& p, vector<vector<bool> >& f,
               vector<int>& soma, int np, double tc)
{
    int i, j, pai, mae, cut;

    tc *= 100;

    i = 0;
    while(i < np)
    {
        pai = roleta(soma, np);
        mae = roleta(soma, np);

        if((rand() % 101) <= tc)
        {
            cut = (rand() % (N - 2)) + 1;
            for(j = 0; j < cut; j++)
            {
                f[i][j] = p[pai][j];
                f[i+1][j] = p[mae][j];
            }

            for(j = cut; j < N; j++)
            {
                f[i][j] = p[mae][j];
                f[i+1][j] = p[pai][j];
            }
        }
        else
        {
            for(j = 0; j < N; j++)
            {
                f[i][j] = p[pai][j];
                f[i+1][j] = p[mae][j];
            }
        }
        i += 2;
    }
}

void multacacao(vector<vector<bool> >& f, int np, double tm)
{
    tm *= 1000;
    for(int i = 0; i < np; i++)
    {
        for(int j = 0; j < N; j++)
        {
            if((rand()% 1001) <= tm)
                f[i][j] = !f[i][j];
        }
    }
}
