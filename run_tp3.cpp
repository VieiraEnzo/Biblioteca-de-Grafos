#include "TP3.h"
#include <iostream>
ofstream file;

void runEx(){
    
    for(int i = 1; i <= 6; i++){
        weighted_vector g1("grafos_tp3/grafo_rf_"+ to_string(i) +".txt",false);
        file.open("Quest_1_" + to_string(i) + ".txt");

        double sum = 0;
        int result;
        for(int i =0; i < 10; i ++){
            clock_t start_time = clock();
            result = g1.Ford_Fulkerson(1,2);
            clock_t end_time = clock();
            double elapsed = (end_time - start_time)/(double)CLOCKS_PER_SEC;
            sum += elapsed;
        }
        file << sum/10 << "\n";
        file << result << "\n";
        file.close();   

    }
}

void runT(){

}


int main()
{
    runEx();

        
}