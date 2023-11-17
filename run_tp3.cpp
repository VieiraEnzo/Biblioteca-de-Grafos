#include "TP3.h"
#include <iostream>
ofstream file;


int main()
{
    weighted_vector a("grafos_tp3/grafo_rf_1.txt",false);
    cout << a.Ford_Fulkerson(1,2,false,"./");
}