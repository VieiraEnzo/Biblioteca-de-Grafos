#include <iostream>
#include <vector>
#include <map>
using namespace std;


class weighted_graph{

    public:

        //Construtor do Grafo, chama a leitura do mesmo
        weighted_graph(string filename);

        //Baseado nas características do grafo roda o algorimo de caminho mínimo adequado
        pair<vector<int>, vector<int>> Caminho_mínimo(int vert);

        // Roda caminho minimo em Dijkstra (usando um vetor) e retorna vetores de distancia e pais de cada vértice
        pair<vector<int>, vector<int>> Dijkstra_vector(int vert);

        // Roda caminho minimo em Dijkstra (usando uma heap) e retorna vetores de distancia e pais de cada vértice
        pair<vector<int>, vector<int>> Dijkstra_heap(int vert);

        //EXTRA: Algoritmo Bellman-Ford
        //Roda caminho minimo em Bellman-Ford e retorna vetores de distancia e pais de cada vértice
        pair<vector<int>, vector<int>> Bellman_Ford(int vert);

        //EXTRA: Algoritmo Floyd-Warshal
        //Roda caminho minimo em Floyd-Warshal e retorna vetores de distancia
        vector<vector<int>> Floyd_Warshal();


    private:
        string graph_name;
        bool negative_weights;
        int num_vertices = -1;
        int num_arestas = -1;
};

//Representação usando matriz de adjascencia
class weighted_matrix{

};

//Representação usando vetor de adjascencia
class weighted_vector{

};

//Representação usando map para guardar vetor (Indices como strings)
class weighted_map{

};