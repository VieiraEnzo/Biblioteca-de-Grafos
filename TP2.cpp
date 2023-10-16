#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <fstream>
using namespace std;
#define f first
#define s second

//Estruturei um pouco o código, algumas estruturas estão meio erradas
//Porque está faltando representar o peso. Criar aresta não implementado.



class weighted_graph{

    public:

        //Construtor do Grafo, chama a leitura do mesmo
        weighted_graph(string filename){
            read_graph(filename);
        }

        //Le o grafo de um arquivo
        void read_graph(string filename){

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
                    int n1 = stoi(line.substr(0, line.find_first_of(" ")));
                    int n2 = stoi(line.substr(line.find_first_of(" ") +1 , line.find_last_of(" ")));
                    double n3 = stod(line.substr(line.find_last_of(" ")+1));
                    if(n3 < 0) this->negative_weights = true;
                    criar_aresta(n1,n2,n3);
                    this->num_arestas++;
                }
                this->num_arestas++;
            }
            
            else throw std::invalid_argument( "Impossivel abrir o arquivo" );
        }


        //Baseado nas características do grafo roda o algorimo de caminho mínimo adequado
        pair<vector<double>&, vector<int>&> Caminho_mínimo(int vert){
            if(!this->negative_weights){
                return this->Dijkstra_heap(vert);

            }else{
                cout << "O grafo possui pesos negativos, logo usaremos o algoritmo de Bellman_Ford" << endl;
                return this->Bellman_Ford(vert);
            }
        }

        // Roda caminho minimo em Dijkstra (usando um vetor) e retorna vetores de distancia e pais de cada vértice
        pair<vector<int>, vector<int>> Dijkstra_vector(int vert);

        // Roda caminho minimo em Dijkstra (usando uma heap) e retorna vetores de distancia e pais de cada vértice
        pair<vector<double>&, vector<int>&> Dijkstra_heap(int vert){
            
            vector<double> dist(num_vertices,1e8);
            vector<int> pai(num_vertices, -1);
            priority_queue<pair<int,double>, vector<pair<int,double>>, greater<pair<int,double>>> pq;
            dist[vert] = 0;
            pai[vert] = vert;

            while (!pq.empty())
            {
                int atual = pq.top().f; int custo = pq.top().s;
                pq.pop();

                if(custo > dist[atual]) continue;

                for(pair<int,double> viz : get_vizinhos(atual)){
                    if(dist[viz.f] > custo + viz.s){
                        pq.push({viz.f, custo + viz.s});
                        dist[viz.f] = custo + viz.s;
                        pai[viz.f] = atual;
                    }
                }
            }

            return {dist, pai};
            
        }

        //EXTRA: Algoritmo Bellman-Ford
        //Roda caminho minimo em Bellman-Ford e retorna vetores de distancia e pais de cada vértice
        pair<vector<double>&, vector<int>&> Bellman_Ford(int vert);

        //EXTRA: Algoritmo Floyd-Warshal
        //Roda caminho minimo em Floyd-Warshal e retorna vetores de distancia
        //A matrix deverá ser (num_vert x num_vert) e a distancia de (i,i) deverá ser 0 para todo i
        //e infinito para arestas inexistentes
        vector<vector<double>>& Floyd_Warshal(vector<vector<double>>& dist){

            for(int k =0; k < num_arestas; k ++)
                for(int i =0; i < num_arestas; i++)
                    for(int j =0; j < num_arestas; j ++)
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);

            return dist;
        }


    protected:
        string graph_name;
        bool negative_weights = false;
        int num_vertices = -1;
        int num_arestas = -1;

        //Após sabermos o número de vértices, atualiza a estrutura
        virtual void atualizar_estrutura();

        //Insere arestas dependendo da respresentação
        virtual void criar_aresta(int n1, int n2, double n3);

        virtual vector<pair<int,double>>& get_vizinhos(int vert);
};


//Representação usando vetor de adjascencia
class weighted_vector: public weighted_graph{

    public:

        //Chamando o construtor da classe base
        weighted_vector(string filename): weighted_graph(filename){}


    private:

        vector<vector<pair<int,double>>> arestas;

        //Após sabermos o número de vértices, atualiza a estrutura
        void atualizar_estrutura(){
            arestas.resize(this->num_vertices);
        }

        //Insere arestas dependendo da respresentação
        void criar_aresta(int n1, int n2, double n3);

        //retorna um ponteiro para o vetor com os vizinhos de vert
        virtual vector<pair<int,double>>& get_vizinhos(int vert){return arestas[vert];}

};

//Representação usando map para guardar vetor (Indices como strings)
class weighted_map: public weighted_graph{


    public:

        //Chamando o construtora da classe base
        weighted_map(string filename): weighted_graph(filename){}


    private:

        map<string,pair<int,double>> arestas;

        //Pela natureza do map, não precisamos atualizar estrutura

        //Insere arestas dependendo da respresentação
        void criar_aresta(int n1, int n2, int n3);

};


//Representação usando matriz de adjascencia
class weighted_matrix: public weighted_graph{


    public:

        weighted_matrix(string filename): weighted_graph(filename){}


    private:

        vector<vector<int>> arestas;

        //Após sabermos o número de vértices, atualiza a estrutura
        void atualizar_estrutura(){
            arestas.resize(this->num_vertices);
            for(vector<int> a : arestas) a.resize(this->num_vertices);
        }

        //Insere arestas dependendo da respresentação
        void criar_aresta(int n1, int n2, int n3);

};
