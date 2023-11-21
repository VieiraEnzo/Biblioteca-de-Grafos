#include "bits/stdc++.h"
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
                            //if(rGraph[parent[start].first][i].second < 0) cout << "negativo" << endl;
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

        int Ford_Fulkerson_map(int s, int t, bool wrtite = false, string path_out = "./"){
                    //creates a residual graph
                    vector<map<int,int>> rGraph(this->num_vertices);
                    for(int i = 0; i < this->num_vertices; i++){
                        for(auto vert: get_vizinhos(i)){
                            // rGraph[i].push_back(make_pair(vert.first, vert.second));
                            // rGraph[vert.first].push_back(make_pair(i, 0));
                            rGraph[i][vert.first] = vert.second;
                        }
                    }
                    int max_flow = 0;

                    vector<pair<int, int>> parent(this-> num_vertices);
                    for(int c = 1 << 30; c > 0; c >>= 1){
                        while (bfs_map(s,t,c,rGraph,parent))
                        {
                            int minResCap = MRC(s,t, parent);
                            max_flow += minResCap;
                            int start = t;
                            
                            //UPDATE DA CAPACIDADE DAS ARESTAS
                            while (start != s)
                            {
                                int next = parent[start].first;

                                rGraph[next][start] -= minResCap;
                                if(rGraph[next][start] == 0) rGraph[next].erase(start);
                                rGraph[start][next] += minResCap;

                                start = parent[start].first;
                            }
                        }
                    }

                    return max_flow;
                }
        
        int Dinitz(int s, int t){
            vector<vector<edge_t>> adj(this->num_vertices);
            vector<int> lvl(this->num_vertices), ptr(this->num_vertices), q(this->num_vertices);

            for(int n1 = 0; n1 < this->num_vertices; n1++){
                for(auto [n2, n3] : get_vizinhos(n1)){
                    adj[n1].push_back({n2, (int)adj[n2].size(), n3, 0});
                    adj[n2].push_back({n1, (int)adj[n1].size()-1, 0, 0});
                }
            }

            int flow = 0; q[0] = s;
            for (int L = 0; L < 31; ++L) do {
                lvl = ptr = vector<int>(q.size());
                int qi = 0, qe = lvl[s] = 1;
                while (qi < qe && !lvl[t]) {
                    int v = q[qi++];
                    for (edge_t &e : adj[v])
                        if (!lvl[e.to] && (e.c - e.f) >> (30 - L))
                            q[qe++] = e.to, lvl[e.to] = lvl[v] + 1;
                }
                while(int p =dfs(s, t, numeric_limits<int>::max()/4, adj, lvl, ptr))flow+=p;
            } while (lvl[t]);
            return flow;
        }


    protected:
        string graph_name = "Graph";;
        bool negative_weights = false;
        int num_vertices = -1;
        int num_arestas = -1;
        vector<int> arestas;
        bool directed = false;
        
        
        struct edge_t { int to, rev; int c, f; };
        

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
                    if(visited[vizinho] || cap <= 0) continue;
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
            while (start != s)
            {
                CapMin = min(CapMin, parent[start].second);
                start = parent[start].first;
            }
            //cout << "CapMin: "<< CapMin << endl;
            return CapMin;
        }

        //used in dinitz
        int dfs(int v, int t, int f, vector<vector<edge_t>> &adj, vector<int> &lvl, vector<int> &ptr) {
            if (v == t || !f) return f;
            for (int &i = ptr[v]; i < int(adj[v].size()); ++i) {
                edge_t &e = adj[v][i];
                if (lvl[e.to] == lvl[v] + 1)
                    if (int p = dfs(e.to, t, min(f, e.c - e.f), adj, lvl, ptr)) {
                        e.f += p, adj[e.to][e.rev].f -= p;
                        return p;
                    }
            } return 0;
        }

        bool bfs_map(int s, int t, int minCap, vector<map<int,int>> &rGraph, vector<pair<int,int>> &parent){
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

                for(auto [vizinho, cap] : rGraph[atual]){
                    if(visited[vizinho] || cap < minCap ) continue;
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