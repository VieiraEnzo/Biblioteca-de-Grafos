#include "TP3.h"
#include <iostream>
ofstream file;

void runEx(){
    
    for(int i = 1; i <= 6; i++){
        weighted_vector g1("grafos_tp3/grafo_rf_"+ to_string(i) +".txt",false);
        file.open("Quest_1_" + to_string(i) + ".txt");

        file << "Grafo " << i << ":\n";
        double sum = 0, sum2 = 0, sum3 = 0;
        int result, result2, result3;
        for(int j =0; j < 10; j++){
            clock_t start_time = clock();
            result = g1.Ford_Fulkerson(1,2);
            clock_t end_time = clock();
            result2 = g1.Ford_Fulkerson_map(1,2);
            clock_t end_time2 = clock();
            result3 = g1.Dinitz(1,2);
            clock_t end_time3 = clock();
            double elapsed = (end_time - start_time)/(double)CLOCKS_PER_SEC;
            double elapsed2 = (end_time2 - end_time)/(double)CLOCKS_PER_SEC;
            double elapsed3 = (end_time3 - end_time2)/(double)CLOCKS_PER_SEC;
            sum += elapsed;
            sum2 += elapsed2;
            sum3 += elapsed3;

            file << j << ": " << elapsed << ' ' << elapsed2 << ' ' << elapsed3 << '\n';
        }
        file << "finais:\n";
        file << sum/10 << "\n";
        file << result << "\n\n";
        file << sum2/10 << '\n';
        file << result2 << "\n\n";
        file << sum3/10 << '\n';
        file << result3 << '\n';
        
        file.close();   

    }
}



int main()
{
    runEx();
    //weighted_vector g1("grafos_tp3/grafo_rf_"+ to_string(1) +".txt", true);
    //cout << g1.Dinitz(1,2) << endl;


        
}