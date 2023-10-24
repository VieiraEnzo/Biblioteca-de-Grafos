#include "TP2.h"
#include <iostream>

void run_ex(){

        for(int i =1; i <=5; i++){
        cout << "Rodando Grafo " << i << "\n";
        weighted_vector g1("./grafos_tp2/grafo_W_" + to_string(i) + ".txt");
        
        //EXERCICIO 1
        
        ofstream file;
        cout << "Rodando EX 1\n";
        file.open("Quest_1_G" + to_string(i) + ".txt");
        auto a = g1.Dijkstra_heap(10);
        //for(auto b : a.first) cout << b << "\n";
        file << a.first[20] << "\n";
        file << a.first[30] << "\n";
        file << a.first[40] << "\n";
        file << a.first[50] << "\n";
        file << a.first[60] << "\n";
        file << "-----------------------------" << "\n";
        cout << "feito" << endl;
        auto b = g1.Dijkstra_vector(10);
        file << b.first[20] << "\n";
        file << b.first[30] << "\n";
        file << b.first[40] << "\n";
        file << b.first[50] << "\n";
        file << b.first[60] << "\n";
        file.close();

        //EXERCICIO 2
        cout << "Rodando EX 2\n";
        file.open("Quest_2_G" + to_string(i) + ".txt");
        double sum = 0;
        for(int i =1; i <= 100; i ++){
            clock_t start_time = clock();
            g1.Dijkstra_heap(5*i);
            clock_t end_time = clock();
            double elapsed = (end_time - start_time)/(double)CLOCKS_PER_SEC;
            sum += elapsed;
        }
        file << sum/100 << "\n";

        // sum = 0;
        // for(int i =1; i <= 100; i ++){
        //     clock_t start_time = clock();
        //     g1.Dijkstra_vector(5*i);
        //     clock_t end_time = clock();
        //     double elapsed = (end_time - start_time)/(double)CLOCKS_PER_SEC;
        //     sum += elapsed;
        // }
        // file << sum/100 << "\n";
        // file.close();
    }

    ofstream file;
    //EXERCICIO 3
    cout << "Rodando EX 3\n";
    file.open("Quest_3.txt");
    weighted_vector g1("rede_colaboracao.txt");
    auto a = g1.Dijkstra_heap(2722); // Dijkstra
    file << a.first[11365] << "\n"; //Turing
    file << a.first[471365] << "\n"; //Kruskal
    file << a.first[5709] << "\n"; //Jon M. Kleinberg
    file << a.first[11386] << "\n"; //Eva Tardos
    file << a.first[343930] << "\n"; //Daniel

    file.close();


}


int main()
{
    //weighted_vector g1("grafos_tp2/grafo0.txt");
    //weighted_vector g1("grafos_tp2/grafo_W_1.txt");
    //auto a = g1.Dijkstra_heap(1);
    //for(auto b : a.first) cout << b << endl;



    run_ex();
    
        
}