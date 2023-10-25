#include "TP2.h"
#include <iostream>
ofstream file;

void caminho_min(int inic, vector<int>& a, int obj){
        while (a[inic] != obj)
        {
            cout << inic << endl;
            file << inic << " ";
            inic = a[inic];
            if(inic == -1){
                file << -1 << endl;
                break;
            }
        }
        file << obj << "\n";
}


void run_ex(){

        for(int i =3; i <=5; i++){
        cout << "Rodando Grafo " << i << "\n";
        weighted_vector g1("./grafos_tp2/grafo_W_" + to_string(i) + ".txt");
        
        //EXERCICIO 1
        
        
        // cout << "Rodando EX 1\n";
        // file.open("Quest_1_G" + to_string(i) + ".txt");
        // auto a = g1.Dijkstra_heap(10);
        // //for(auto b : a.first) cout << b << "\n";
        // file << a.first[20] << "\n";
        // caminho_min(20, a.second, 10);
        // file << a.first[30] << "\n";
        // caminho_min(30, a.second, 10);
        // file << a.first[40] << "\n";
        // caminho_min(40, a.second, 10);
        // file << a.first[50] << "\n";
        // caminho_min(50, a.second, 10);
        // file << a.first[60] << "\n";
        // caminho_min(60, a.second, 10);
        // file << "-----------------------------" << "\n";
        // auto b = g1.Dijkstra_vector(10);
        // file << b.first[20] << "\n";
        // caminho_min(20, b.second, 10);
        // file << b.first[30] << "\n";
        // caminho_min(30, b.second, 10);
        // file << b.first[40] << "\n";
        // caminho_min(40, b.second, 10);
        // file << b.first[50] << "\n";
        // caminho_min(50, b.second, 10);
        // file << b.first[60] << "\n";
        // caminho_min(60, b.second, 10);
        // file.close();

        // EXERCICIO 2
        cout << "Rodando EX 2\n";
        file.open("Quest_2.2_G" + to_string(i) + ".txt");

        double sum = 0;
        for(int i =1; i <= 100; i ++){
            clock_t start_time = clock();
            g1.Dijkstra_heap(5*i);
            clock_t end_time = clock();
            double elapsed = (end_time - start_time)/(double)CLOCKS_PER_SEC;
            sum += elapsed;
        }
        file << sum/100 << "\n";
        file.close();
        
        // cout << "parte 2 ex 2" <<endl;
        // res.clear();
        // #pragma omp parallel for
        // for(int i =1; i <= 1; i ++){
        //     clock_t start_time = clock();
        //     g1.Dijkstra_vector(5*i);
        //     clock_t end_time = clock();
        //     double elapsed = (end_time - start_time)/(double)CLOCKS_PER_SEC;
        //     res.push_back(elapsed);
        //}
        // sum = 0;
        // for(auto a : res) sum+= a;
        // file << sum/1 << "\n";
        // file.close();
    }

    //EXERCICIO 3
    // cout << "Rodando EX 3\n";
    // file.open("Quest_3.txt");
    // weighted_vector g1("grafos_tp2/rede_colaboracao.txt");
    // auto a = g1.Dijkstra_heap(2722); // Dijkstra
    // //for(auto b : a.second) cout << b << endl;
    // file << a.first[11365] << "\n"; //Turing
    // caminho_min(11365, a.second, 2722);
    // file << a.first[471365] << "\n"; //Kruskal
    // caminho_min(471365, a.second, 2722);
    // file << a.first[5709] << "\n"; //Jon M. Kleinberg
    // caminho_min(5709, a.second, 2722);
    // file << a.first[11386] << "\n"; //Eva Tardos
    // caminho_min(11386, a.second, 2722);
    // file << a.first[343930] << "\n"; //Daniel
    // caminho_min(343930, a.second, 2722);


    // file.close();


}


int main()
{
    //weighted_vector g1("grafos_tp2/grafo0.txt");
    //weighted_vector g1("grafos_tp2/grafo_W_1.txt");
    //auto a = g1.Dijkstra_vector(1);
    //for(auto b : a.first) cout << b << endl;
    //cout << a.first[410] << endl;



    run_ex();
    
        
}