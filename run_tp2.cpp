#include "TP2.h"

int main()
{
    weighted_vector g0("./grafos_tp2/grafo0.txt");
    auto a = g0.Dijkstra_heap(1);
    for(auto b : a.first){
        cout << b << endl;
    }
}