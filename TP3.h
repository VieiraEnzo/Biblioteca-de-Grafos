#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <fstream>
using namespace std;

//Tem um problema no dyikstra_vector no retorno da posição 1



class weighted_graph{

    public:

        //Le o grafo de um arquivo
        void read_graph(string filename){

            cout << "Lendo grafo " << graph_name << endl;
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
                    int n3 = stoi(line.substr(line.find_last_of(" ")+1));
                    if(n3 < 0) this->negative_weights = true;
                    criar_aresta(n1,n2,n3);
                    this->num_arestas++;
                }
                this->num_arestas++;
            }
            
            else throw std::invalid_argument( "Impossivel abrir o arquivo" );
        }


        // retornar o valor do fluxo e a alocação de fluxo em cada aresta, com uma opção de armazenar o resultado em disco
        int Ford_Fulkerson(int s, int t, bool write = false, string path_out = "./"){
            
            //creates a residual graph
            vector<vector<pair<int,int>>> rGraph(this->num_vertices);
            for(int i = 0; i < this->num_vertices; i++){
                for(auto vert: get_vizinhos(i)){
                    rGraph[i].push_back(make_pair(vert.first, vert.second));
                    rGraph[vert.first].push_back(make_pair(i, 0));
                }
            }
            int max_flow = 0;

            vector<pair<int, int>> parent(this-> num_vertices);
            while (bfs(s,t,rGraph,parent))
            {
                int minResCap = MRC(s,t, parent);
                max_flow += minResCap;
                int start = t;
                
                //UPDATE DA CAPACIDADE DAS ARESTAS
                while (parent[start].first != start)
                {
                    //cout << "pai vertice: "<< parent[start].first << endl;
                    for(int i = 0 ; i < rGraph[parent[start].first].size(); i++){
                        if(rGraph[parent[start].first][i].first == start){
                            //cout << "cap antes: " << rGraph[parent[start].first][i].second << endl;
                            rGraph[parent[start].first][i].second -= minResCap;
                            //cout << "cap despois: " << rGraph[parent[start].first][i].second << endl;
                            break;
                            }
                    }

                    for(int i = 0 ; i < rGraph[start].size(); i++){
                        if(rGraph[parent[start].first][i].first == start){
                            //cout << "cap antes: " << rGraph[start][i].second << endl;
                            rGraph[start][i].second += minResCap;
                            //cout << "cap despois: " << rGraph[start][i].second << endl;
                            break;
                            }
                    }

                    start = parent[start].first;
                    // for(auto a : rGraph){
                    //     for(auto b : a)
                    //         cout << b.first << " " << b.second << " ;"<< " ";
                    //     cout << endl;
                    // }
                }



            }

            return max_flow;
        }

    protected:
        string graph_name = "Graph";;
        bool negative_weights = false;
        int num_vertices = -1;
        int num_arestas = -1;
        vector<int> arestas;
        bool directed = false;
        

        //Após sabermos o número de vértices, atualiza a estrutura
        virtual void atualizar_estrutura() = 0;

        //Insere arestas dependendo da respresentação
        virtual void criar_aresta(int n1, int n2, int n3) = 0;

        virtual vector<pair<int,int>>& get_vizinhos(int vert) = 0;

        //Usado no Ford_Fulkerson para encontrar caminhos aumentantes
        bool bfs(int s, int t, vector<vector<pair<int,int>>> rGraph, vector<pair<int,int>> &parent){
            
            vector<bool> visited(this->num_vertices,false);
            fill(parent.begin(), parent.end(), make_pair(-1,-1));
            visited[s] = true;
            parent[s].first = s;
            parent[s].second = 0;
            queue<int> fila;
            fila.push(s);

            while (!fila.empty())
            {
                int atual = fila.front();
                fila.pop();

                for(auto viz : rGraph[atual]){
                    int vizinho = viz.first; int cap = viz.second;
                    if(visited[vizinho] || cap == 0) continue;
                    //cout << vizinho << endl;
                    fila.push(vizinho);
                    visited[vizinho] = true;
                    parent[vizinho].first = atual;
                    parent[vizinho].second = cap;
                    if(vizinho == t){return true;}
                }
            }

            return false;
        }

        //find minimal residual capacity of a path
        int MRC(int s, int t, vector<pair<int,int>> parent){
            int start = t; int CapMin = 1e9;
            while (parent[start].first != s)
            {
                CapMin = min(CapMin, parent[start].second);
                start = parent[start].first;
            }
            //cout << "CapMin: "<< CapMin << endl;
            return CapMin;
        }

};


//Representação usando vetor de adjascencia
class weighted_vector: public weighted_graph{

    public:

        //Chamando o construtor da classe base
        weighted_vector(string filename, bool directed_v = false){
            this->graph_name = filename;
            this->directed = directed_v;
            read_graph(filename);
        }


    private:

        vector<vector<pair<int,int>>> arestas;

        //Após sabermos o número de vértices, atualiza a estrutura
        void atualizar_estrutura() override {
            arestas.resize(num_vertices);
        }

        //Insere arestas dependendo da respresentação, levando em conta a direção do grafo
        void criar_aresta(int n1, int n2, int n3) override {
            arestas[n1].push_back(make_pair(n2,n3));
            if(directed) arestas[n2].push_back(make_pair(n1,n3));
        }

        //retorna um ponteiro para o vetor com os vizinhos de vert
        virtual vector<pair<int,int>>& get_vizinhos(int vert){return arestas[vert];}

};