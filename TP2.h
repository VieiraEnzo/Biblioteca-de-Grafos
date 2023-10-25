#include <iostream>
#include <vector>
#include <queue>
#include <map>
#include <fstream>
using namespace std;
#define f first
#define s second

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
        pair<vector<double>, vector<int>> Caminho_mínimo(int vert){
            if(!this->negative_weights){
                return this->Dijkstra_heap(vert);

            }else{
                cout << "O grafo possui pesos negativos, logo usaremos o algoritmo de Bellman_Ford" << endl;
                return this->Bellman_Ford(vert);
            }
        }

        // Roda caminho minimo em Dijkstra (usando um vetor) e retorna vetores de distancia e pais de cada vértice
        pair<vector<double>, vector<int>> Dijkstra_vector(int vert){

            vector<double> dist(num_vertices,1e9);
            vector<int> pai(num_vertices, -1);
            vector<pair<double,int>> vc;
            dist[vert] = 0;
            pai[vert] = vert;
            vc.push_back(make_pair(0,vert));

            while (!vc.empty())
            {
                
                int atual = 1, index = 0; double custo = 1e9; 
                for(int i = 0; i < vc.size(); i++){
                    if(vc[i].f < custo){
                        custo = vc[i].f;
                        atual = vc[i].s;
                        index = i;
                    }
                }
                vc.erase(vc.begin() + index);
                

                if(custo > dist[atual]) continue;

                for(pair<int,double> viz : get_vizinhos(atual)){
                    //get vizinhos devolve um vetor {aresta,peso}
                    if(dist[viz.f] > custo + viz.s){
                        vc.push_back({custo + viz.s, viz.f});
                        dist[viz.f] = custo + viz.s;
                        pai[viz.f] = atual;
                    }
                }
            }
            return {dist, pai};

        }

        // Roda caminho minimo em Dijkstra (usando uma heap) e retorna vetores de distancia e pais de cada vértice
        pair<vector<double>, vector<int>> Dijkstra_heap(int vert){
            
            vector<double> dist(num_vertices,1e9);
            vector<int> pai(num_vertices, -1);
            priority_queue<pair<double,int>, vector<pair<double,int>>, greater<pair<double,int>>> pq;
            dist[vert] = 0;
            pai[vert] = vert;
            pq.push(make_pair(0,vert));

            while (!pq.empty())
            {
                int atual = pq.top().s; double custo = pq.top().f;
                pq.pop();

                if(custo > dist[atual]) continue;

                for(pair<int,double> viz : get_vizinhos(atual)){
                    //get vizinhos devolve um vetor {aresta,peso}
                    if(dist[viz.f] > custo + viz.s){
                        pq.push({custo + viz.s, viz.f});
                        dist[viz.f] = custo + viz.s;
                        pai[viz.f] = atual;
                    }
                }
            }
            return {dist, pai};
            
        }

        //EXTRA: Algoritmo Bellman-Ford
        //Roda caminho minimo em Bellman-Ford e retorna vetores de distancia e pais de cada vértice
        pair<vector<double>, vector<int>> Bellman_Ford(int vert){
            vector<double> dist(num_vertices, 1e9);
            vector<int> pai(num_vertices, -1);

            vector<pair<double, pair<int,int>>> lista_arestas;

            for(int i = 0; i < num_vertices; i++){
                for(pair<int,double> viz : get_vizinhos(i)){
                    //get vizinhos devolve um vetor {aresta,peso}
                    lista_arestas.push_back( {viz.first, {i, viz.second}} );
                    dist[vert] = 0;
                    pai[vert] = vert;
                }
            }

            for(int k = 1; k < num_vertices; k++){
                for(int i = 0; i < lista_arestas.size(); i++){
                    int a = lista_arestas[i].second.first, b = lista_arestas[i].second.second;
                    double c = lista_arestas[i].first;
                    
                    //checa se dist[a] é infinita ou se a aresta atual não possui uma melhora
                    if( abs(dist[a] - 1e9) < 1e-9 || dist[b] < dist[a] + c || abs(dist[b] - (dist[a] + c)) < 1e-9 ) continue;
                    dist[b] = dist[a] + c;
                    pai[b] = a;
                }
            }

            bool negative_cycle = false;
            for(int i = 0; i < lista_arestas.size(); i++){
                int a = lista_arestas[i].second.first, b = lista_arestas[i].second.second;
                double c = lista_arestas[i].first;

                if( abs(dist[a] - 1e9) < 1e-9 || dist[b] < dist[a] + c || abs(dist[b] - (dist[a] + c)) < 1e-9 ) continue;
                
                negative_cycle = true;
                break;
            }

            //Se há um ciclo negativo, todos os vértices que vert alcança possuem distância mínima infinitamente pequena
            if(negative_cycle){
                for(int i = 0; i < num_vertices; i++){
                    if(abs(dist[i] - 1e9) > 1e-9) dist[i] = -1e9;
                }
            }

            return {dist, pai};
        }

        //EXTRA: Algoritmo Floyd-Warshal
        //Roda caminho minimo em Floyd-Warshal e retorna vetores de distancia
        //A matrix deverá ser (num_vert x num_vert) e a distancia de (i,i) deverá ser 0 para todo i
        //e infinito para arestas inexistentes
        vector<vector<double>> Floyd_Warshal(vector<vector<double>>& dist){

            cout << "RodandoFloyd_Warshal O(n^3)" << endl;
            for(int k =0; k < num_arestas; k ++)
                for(int i =0; i < num_arestas; i++)
                    for(int j =0; j < num_arestas; j ++)
                        dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);

            return dist;
        }


    protected:
        string graph_name = "Graph";;
        bool negative_weights = false;
        int num_vertices = -1;
        int num_arestas = -1;
        vector<int> arestas;
        

        //Após sabermos o número de vértices, atualiza a estrutura
        virtual void atualizar_estrutura() = 0;

        //Insere arestas dependendo da respresentação
        virtual void criar_aresta(int n1, int n2, double n3) = 0;

        virtual vector<pair<int,double>>& get_vizinhos(int vert) = 0;
};


//Representação usando vetor de adjascencia
class weighted_vector: public weighted_graph{

    public:

        //Chamando o construtor da classe base
        weighted_vector(string filename){
            this->graph_name = filename;
            read_graph(filename);
        }


    private:

        vector<vector<pair<int,double>>> arestas;

        //Após sabermos o número de vértices, atualiza a estrutura
        void atualizar_estrutura() override {
            arestas.resize(num_vertices);
        }

        //Insere arestas dependendo da respresentação
        void criar_aresta(int n1, int n2, double n3) override {
            arestas[n1].push_back(make_pair(n2,n3));
            arestas[n2].push_back(make_pair(n1,n3));
        }

        //retorna um ponteiro para o vetor com os vizinhos de vert
        virtual vector<pair<int,double>>& get_vizinhos(int vert){return arestas[vert];}

};

//Representação usando map para guardar vetor (Indices como strings)
class weighted_map: public weighted_graph{


    public:

        //Chamando o construtora da classe base
        weighted_map(string filename){
            this->graph_name = filename;
            read_graph(filename);
        }


    private:

        map<string,pair<int,double>> arestas;

        //Pela natureza do map, não precisamos atualizar estrutura

        //Insere arestas dependendo da respresentação
        void criar_aresta(int n1, int n2, int n3);

};


//Representação usando matriz de adjascencia
class weighted_matrix: public weighted_graph{


    public:

        weighted_matrix(string filename){
            this->graph_name = filename;
            read_graph(filename);
        }


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
