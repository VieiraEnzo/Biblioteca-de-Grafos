#include <iostream>
#include <vector>
#include <map>
#include <fstream>
using namespace std;


class weighted_graph{

    public:

        //Construtor do Grafo, chama a leitura do mesmo
        weighted_graph(string filename){
            cout << "oi";
        }

        //Le o grafo de um arquivo
        void read_graph(string filename){

            cout << "Lendo grafo" << endl;
            ifstream file;
            string line;
            file.open(filename, ios::in);


            if(file.is_open())
            {   

                getline(file, line);
                this->num_vertices = stoi(line.substr(0,line.find_last_of("\n")));
                this->num_vertices++;
                atualizar_estrutura();
                while (getline(file,line))
                {   
                    if(line.size()<1)continue;
                    int n1 = stoi(line.substr(line.find_last_of(" ")+1));
                    int n2 = stoi(line.substr(0, line.find_last_of(" ")));
                    int n3 = 0;
                    inserir_vertice(n1,n2,n3);
                    this->num_arestas++;
                }
                this->num_arestas++;
            }
            
            else throw std::invalid_argument( "Impossivel abrir o arquivo" );
        }

        


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


        virtual void atualizar_estrutura();

        virtual void inserir_vertice(int n1, int n2, int n3);
};

//Representação usando matriz de adjascencia
class weighted_matrix: public weighted_graph{

    public:

        weighted_matrix(string filename): weighted_graph(filename){

        }


    private:

        vector<vector<int>> arestas;

        //Após sabermos o número de vértices, atualiza a estrutura
        void atualizar_estrutura(){
            arestas.resize(this->num_vertices);
            for(vector<int> a : arestas) a.resize(this->num_vertices);
        }

};

//Representação usando vetor de adjascencia
class weighted_vector: public weighted_graph{

    public:

        //Chamando o construtor da classe base
        weighted_vector(string filename): weighted_graph(filename){}



    private:

        vector<vector<int>> arestas;

        //Após sabermos o número de vértices, atualiza a estrutura
        void atualizar_estrutura(){
            arestas.resize(this->num_vertices);
        }

};

//Representação usando map para guardar vetor (Indices como strings)
class weighted_map: public weighted_graph{

    public:

        //Chamando o construtora da classe base
        weighted_map(string filename): weighted_graph(filename){}

    private:

        map<string,vector<int>> arestas;

        //Pela natureza do map, não precisamos atualizar estrutura

};