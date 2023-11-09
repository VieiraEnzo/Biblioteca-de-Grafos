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


        // retornar o valor do fluxo e a alocação de fluxo em cada aresta, com uma opção de armazenar o resultado em disco
        pair<vector<int>, vector<int>> Ford_Fulkerson(int v, bool write = false, string path_out = "./");

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
        virtual void criar_aresta(int n1, int n2, double n3) = 0;

        virtual vector<pair<int,double>>& get_vizinhos(int vert) = 0;

};


//Representação usando vetor de adjascencia
class weighted_vector: public weighted_graph{

    public:

        //Chamando o construtor da classe base
        weighted_vector(string filename, bool directed_v){
            this->graph_name = filename;
            this->directed = directed_v;
            read_graph(filename);
        }


    private:

        vector<vector<pair<int,double>>> arestas;

        //Após sabermos o número de vértices, atualiza a estrutura
        void atualizar_estrutura() override {
            arestas.resize(num_vertices);
        }

        //Insere arestas dependendo da respresentação, levando em conta a direção do grafo
        void criar_aresta(int n1, int n2, double n3) override {
            arestas[n1].push_back(make_pair(n2,n3));
            if(directed) arestas[n2].push_back(make_pair(n1,n3));
        }

        //retorna um ponteiro para o vetor com os vizinhos de vert
        virtual vector<pair<int,double>>& get_vizinhos(int vert){return arestas[vert];}

};