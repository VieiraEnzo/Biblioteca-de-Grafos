#include <iostream>
#include <fstream>
#include <vector>
#include <stack>
#include <queue>
#include <list>
#include <algorithm>
#include "windows.h"
#include "psapi.h"
#include <ctime>
#include <omp.h>
using namespace std;
#define INF 1e9;


//Otimização em distancia procurar se o arquivo de v1 ou v2 já foram gerados

class graph{
    public:  

        //le o grafo de um arquivo
        void read_graph(string filename);

        //Realiza busca a partir de um vértice informado, retorna a árvore gerada (por meio do pai)
        //assim como o nível de cada vértice da árvore (raiz é 0). Escrevendo em um arquivo;
        void bfs(int v);

        //Realiza busca em profundidade 
        //void dfs_recursive(int v){}

        void dfs_norecursive(int v);

        //calcula a distancia entre 2 vértices do grafo
        int distancia(int v1, int v2);

        //calcula o diametro do grafo
        int calc_diametro();

        //retorna o numero de componetes, o tamanho de cada e a lista de vértices que participam dele
        void componentes_conexas();
        
        
        //escreve em um arquivo o numero de vertices, numero de arestas, grau min, grau max, grau medio e mediana do grau
        void escrever_grafo();


        void create_degree();

    protected:

        string graph_name;

        int num_arestas = -1;
        //numero de vertices somado de 1 para indexar em 1;
        int num_vertices = -1;

        int grau_min = -1;
        int grau_max = -1;
        double grau_medio = -1;
        double grau_mediana = -1;

        vector<vector<int>> list_comp_conex;
        vector<int> rel_comp_conex;
        vector<bool> tree_comp_conex;

        int diametro = -1;

        int vizinhos(int i);
};

class graph_vetor : public graph{
    public:

        graph_vetor(string a)
        {   
            this->graph_name = a + "";
            read_graph(a);
        }


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
                arestas.resize(num_vertices);
                while (getline(file,line))
                {   
                    if(line.size()<1)continue;
                    int n1 = stoi(line.substr(line.find_last_of(" ")+1));
                    int n2 = stoi(line.substr(0, line.find_last_of(" ")));
                    arestas[n1].push_back(n2);
                    arestas[n2].push_back(n1);
                    this->num_arestas++;
                }
                this->num_arestas++;
            }
            
            else throw std::invalid_argument( "Impossivel abrir o arquivo" );

        }


        void bfs(int v){
                cout << "Iniciando bfs: O(n+m)" << endl;
                //roda a bfs
                vector<bool> visitado(num_vertices,false);
                vector<int> pai(num_vertices,-1);
                vector<int> nivel(num_vertices,-1); 
                queue<int> fila;
                fila.push(v);
                visitado[v] = true;
                nivel[v] = 0;
                pai[v] = v;

                while (!fila.empty())
                {
                    int vert = fila.front();
                    fila.pop();
                    for(int a : arestas[vert]){
                        if(visitado[a]) continue;
                        fila.push(a);
                        visitado[a] = true;
                        pai[a] = vert;
                        nivel[a] = nivel[vert] + 1;
                    }
                }

                //escreve os dados gerados em um arquivo
                cout << "escrevendo os dados" << endl;
                string a = "./"+graph_name+"_bfs_ " + to_string(v);
                ofstream file(a);
                file << "pai,nivel" << endl;
                for(int i = 1; i < this->num_vertices; i ++){
                    file << to_string(pai[i]) + " " + to_string(nivel[i]) << "\n";
                }

        }

        bool bipartido()
        {
            vector<int> C(this->num_vertices,0);

            for(int i =1; i < this->num_vertices; i++)
                {
                    if(C[i]) continue;
                    queue<int> fila;
                    fila.push(i);
                    C[i] = 1;

                    while (!fila.empty())
                    {
                        int v = fila.front();
                        fila.pop();

                        for(int a : arestas[v])
                        {
                            if(C[a] && C[a] == C[v]) {return false;}
                            if(C[a] && C[a] != C[v]) continue;
                            fila.push(a);
                            C[a] = (C[v]%2)+1;
                        }

                    }
                
                }
            return true;
        }

        void dfs_norecursive(int v)
        {
            vector<bool> visitado(this->num_vertices, false);
            vector<int> pai(num_vertices,-1);
            vector<int> nivel(num_vertices,-1);
            stack<int> pilha;

            nivel[v] = 0;
            pai[v] = v;
            pilha.push(v);

            while (!pilha.empty()) {
                int vertice = pilha.top();
                pilha.pop();
                if(!visitado[vertice]){
                    visitado[vertice] = true;
                    for (int vizinho : arestas[vertice]) {
                        if (!visitado[vizinho]) {
                            pai[vizinho] = vertice;
                            nivel[vizinho] = nivel[vertice] + 1;
                            pilha.push(vizinho);
                        }
                        else{
                            if(vizinho != pai[vertice])
                        }
                    }
                }
            }


            //escreve os dados gerados em um arquivo
            string a = "./"+graph_name+"_dfs_ " + to_string(v);
            ofstream file(a);
            file << "pai,nivel\n";
            for(int i = 1; i < this->num_vertices; i ++){
                file << to_string(pai[i]) + " " + to_string(nivel[i]) << "\n";
            }

        }


        int distancia(int v1, int v2){
            bfs(v1);
            string line;

            string a = "./"+graph_name+"_bfs_ " + to_string(v1);
            ifstream file(a);

            int i =0;
            while (getline(file,line))
            {
                if(i == v2){
                    return stoi(line.substr(line.find_last_of(" ")+1));
                }
                i++;
            }
            return INF;
        }


        int calc_diametro(){
            for(int i = 1; i < num_vertices; i++){
                vector<bool> visitado(num_vertices, false);

                queue<pair<int,int>> fila;
                fila.push(make_pair(i,0));
                visitado[i] = true;

                while (!fila.empty())
                {
                    int vert = fila.front().first;
                    int nivel = fila.front().second;
                    fila.pop();

                    for(int a : arestas[vert]){
                        if(visitado[a]) continue;
                        this->diametro = max(this->diametro, nivel+1);
                        fila.push(make_pair(a,nivel+1));
                        visitado[a] = true;
                    }
                }

            }
            return this->diametro;
        }

        int calc_diametro2(){
            isTree();

            for(int k = 0; k < list_comp_conex.size(); k++){
                if(!tree_comp_conex[k]){
                    for(int i: list_comp_conex[k]){
                        vector<bool> visitado(num_vertices, false);

                        queue<pair<int,int>> fila;
                        fila.push(make_pair(i,0));
                        visitado[i] = true;

                        while (!fila.empty())
                        {
                            int vert = fila.front().first;
                            int nivel = fila.front().second;
                            fila.pop();

                            for(int a : arestas[vert]){
                                if(visitado[a]) continue;
                                this->diametro = max(this->diametro, nivel+1);
                                fila.push(make_pair(a,nivel+1));
                                visitado[a] = true;
                            }
                        }
                    }
                }
                else{
                    stack<int> pilha;
                } 
            }
            for(int i = 1; i < num_vertices; i++){
                vector<bool> visitado(num_vertices, false);

                queue<pair<int,int>> fila;
                fila.push(make_pair(i,0));
                visitado[i] = true;

                while (!fila.empty())
                {
                    int vert = fila.front().first;
                    int nivel = fila.front().second;
                    fila.pop();

                    for(int a : arestas[vert]){
                        if(visitado[a]) continue;
                        this->diametro = max(this->diametro, nivel+1);
                        fila.push(make_pair(a,nivel+1));
                        visitado[a] = true;
                    }
                }

            }
            return this->diametro;
        }
        

        void componentes_conexas(){
            vector<bool> visitado(num_vertices, false);
            this->rel_comp_conex.resize(num_vertices, 0);
            for(int i = 1; i < num_vertices; i++){
                if(visitado[i])continue;
                this->list_comp_conex.resize(list_comp_conex.size()+1);

                queue<int> fila;
                fila.push(i);
                visitado[i] = true;

                while (!fila.empty())
                {
                    int vert = fila.front();
                    rel_comp_conex[i] = list_comp_conex.size();
                    list_comp_conex.back().push_back(vert);
                    fila.pop();
                    for(int a : arestas[vert]){
                        if(visitado[a]) continue;
                        fila.push(a);
                        visitado[a] = true;
                    }
                }
            }
            // sort(list_comp_conex.rbegin(), list_comp_conex.rend());
        }

        void isTree(){
            this->tree_comp_conex.resize(this->list_comp_conex.size(), 1);

            for(int i = 0; i < list_comp_conex.size(); i++){
                int v = list_comp_conex[i][0];
                vector<bool> visitado(this->num_vertices, false);
                vector<int> pai(num_vertices,-1);
                stack<int> pilha;


                pai[v] = v;
                pilha.push(v);

                while (!pilha.empty()) {
                    int vertice = pilha.top();
                    pilha.pop();
                    if(!visitado[vertice]){
                        visitado[vertice] = true;
                        for (int vizinho : arestas[vertice]) {
                            if (!visitado[vizinho]) {
                                pai[vizinho] = vertice;
                                nivel[vizinho] = nivel[vertice] + 1;
                                pilha.push(vizinho);
                            }
                            else{
                                if(vizinho != pai[vertice]){
                                    tree_comp_conex[i] = false;
                                    break;
                                }
                            }
                        }
                    }
                    if(!tree_comp_conex[i]) break;
                }
            }
        }


        void create_degree(){
            
            vector<int> graus;
            for(int i = 1; i < num_vertices; i++){
                graus.push_back(this->vizinhos(i));
            }
            sort(graus.begin(), graus.end());
            this->grau_max = graus.back();
            this->grau_min = graus.front();
            this->grau_medio = (double)2 * ((double)this->num_arestas/(double)(this->num_vertices-1));
            if(graus.size()%2 == 1){
                this->grau_mediana = graus[graus.size()/2];
            }
            else{
                this->grau_mediana = ((double)(graus[graus.size()/2]) + (double)(graus[(graus.size()/2)-1]))/(double)2;
            }
        }
    
        void escrever_grafo(){
            cout << "Criando graus dos vertices" << endl;
            create_degree();
            cout << "Calculando componentes conexas" << endl;
            componentes_conexas();
            
            cout << "Escrevendo Saida" << endl;
            ofstream file("./" + graph_name + "_saida.txt");
            file << "numero de vértices: " << this->num_vertices - 1 << "\n";
            file << "numero de arestas: " << this->num_arestas << "\n";
            file << "grau mínimo: " << this->grau_min << "\n";
            file << "grau máximo: " << this->grau_max << "\n";
            file << "grau médio: " << this->grau_medio << "\n";
            file << "mediana do grau : " << this->grau_mediana << "\n";
            file << "é bipartido: " << bipartido() << "\n";
            file << "numero de componentes conexas: " << this->list_comp_conex.size() << "\n";
            file << "tamanho e componentes conexas:" << "\n\t";
            for(vector<int> a : list_comp_conex){
                file << a.size() << ":  ";
                for(int b : a){
                    file << b << " ";
                }
                file << "\n\t";
            }
        }
    
    private:

        vector<vector<int>> arestas;
        int vizinhos(int i){
            return arestas[i].size();
        }
};

class graph_matrix : public graph{
    public:

        graph_matrix(string a)
        {   
            this->graph_name = a + "";
            read_graph(a);
        }


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
                arestas.resize(num_vertices);
                for(int i = 0; i < num_vertices; i++)arestas[i].resize(num_vertices,0);

                while (getline(file,line))
                {
                    if(line.size()<1)continue;
                    int n1 = stoi(line.substr(line.find_last_of(" ")+1));
                    int n2 = stoi(line.substr(0, line.find_last_of(" ")));
                    arestas[n1][n2] = 1;
                    arestas[n2][n1] = 1;
                    this->num_arestas++;
                }
                this->num_arestas++;

            }   
            else throw std::invalid_argument( "Impossivel abrir o arquivo" );

        }


        void bfs(int v){
                //roda a bfs
                vector<bool> visitado(num_vertices,false);
                vector<int> pai(num_vertices,-1);
                vector<int> nivel(num_vertices,-1); 
                queue<int> fila;
                fila.push(v);
                visitado[v] = true;
                nivel[v] = 0;
                pai[v] = v;

                while (!fila.empty())
                {
                    int vert = fila.front();
                    fila.pop();
                    for(int i =1 ; i < num_vertices; i++){
                        if(visitado[i] || !arestas[vert][i]) continue;
                        fila.push(i);
                        visitado[i] = true;
                        pai[i] = vert;
                        nivel[i] = nivel[vert] + 1;
                    }
                }

                //escreve os dados gerados em um arquivo
                cout << "escrevendo os dados" << endl;
                string a = "./"+graph_name+"_bfs_ " + to_string(v);
                ofstream file(a);
                file << "pai,nivel" << endl;
                for(int i = 1; i < this->num_vertices; i ++){
                    file << to_string(pai[i]) + " " + to_string(nivel[i]) << "\n";
                }

        }


        bool bipartido()
        {
            vector<int> C(this->num_vertices,0);

            for(int i =1; i < this->num_vertices; i++)
                {
                    if(C[i]) continue;
                    queue<int> fila;
                    fila.push(i);
                    C[i] = 1;

                    while (!fila.empty())
                    {
                        int v = fila.front();
                        fila.pop();

                    for(int a =1 ; a < num_vertices; a++){
                        if(!arestas[v][a]) continue;
                        if(C[a] && C[a] == C[v]) {return false;}
                        if(C[a] && C[a] != C[v]) continue;
                        fila.push(a);
                        C[a] = (C[v]%2)+1;
                    }
                    }
                }
            return true;
        }

        void dfs_norecursive(int v)
        {
            vector<bool> visitado(this->num_vertices, false);
            vector<int> pai(num_vertices,-1);
            vector<int> nivel(num_vertices,-1);
            stack<int> pilha;

            visitado[v] = true;
            nivel[v] = 0;
            pai[v] = v;
            pilha.push(v);

            while (!pilha.empty()) {
                int vertice = pilha.top();
                pilha.pop();
                if(!visitado[vertice]){
                    visitado[vertice] = true;
                    for(int i =0; i < num_vertices; i++){
                        if(visitado[i] || !arestas[vertice][i]) continue;
                        pai[i] = vertice;
                        nivel[i] = nivel[vertice] + 1;
                        pilha.push(i);
                    }
                }

                
            }

            //escreve os dados gerados em um arquivo
            string a = "./"+graph_name+"_dfs_ " + to_string(v);
            ofstream file(a);
            file << "pai,nivel\n";
            for(int i = 1; i < this->num_vertices; i ++){
                file << to_string(pai[i]) + " " + to_string(nivel[i]) << "\n";
            }

        }


        int distancia(int v1, int v2){
            bfs(v1);
            string line;

            string a = "./"+graph_name+"_bfs_ " + to_string(v1);
            ifstream file(a);

            int i =0;
            while (getline(file,line))
            {
                if(i == v2){
                    return stoi(line.substr(line.find_last_of(" ")+1));
                }
                i++;
            }
            return INF;
        }

        int calc_diametro(){
            
            vector<bool> visitado(num_vertices, false);
            for(int i = 1; i < num_vertices; i++){
                
                fill(visitado.begin(),visitado.end(), false);
                queue<pair<int,int>> fila;
                fila.push(make_pair(i,0));
                visitado[i] = true;

                while (!fila.empty())
                {
                    int vert = fila.front().first;
                    int nivel = fila.front().second;
                    fila.pop();

                    for(int i = 1; i < num_vertices; i ++){
                        if(visitado[i]|| !arestas[vert][i]) continue;
                        this->diametro = max(this->diametro, nivel+1);
                        fila.push(make_pair(i,nivel+1));
                        visitado[i] = true;
                    }
                }

            }
            return this->diametro;
        }
        
        void componentes_conexas(){
            vector<bool> visitado(num_vertices, false);
            for(int i = 1; i < num_vertices; i++){
                if(visitado[i])continue;
                this->list_comp_conex.resize(list_comp_conex.size()+1);

                queue<int> fila;
                fila.push(i);
                visitado[i] = true;

                while (!fila.empty())
                {
                    int vert = fila.front();
                    list_comp_conex.back().push_back(vert);
                    fila.pop();
                    for(int i = 1; i < num_vertices; i++){
                        if(visitado[i] || !arestas[vert][i]) continue;
                        fila.push(i);
                        visitado[i] = true;
                    }
                }

            }
            sort(list_comp_conex.rbegin(), list_comp_conex.rend());

        }

        void create_degree(){
            
            vector<int> graus;
            for(int i = 1; i < num_vertices; i++){
                graus.push_back(this->vizinhos(i));
            }
            sort(graus.begin(), graus.end());
            this->grau_max = graus.back();
            this->grau_min = graus.front();
            this->grau_medio = (double)2 * ((double)this->num_arestas/(double)(this->num_vertices-1));
            if(graus.size()%2 == 1){
                this->grau_mediana = graus[graus.size()/2];
            }
            else{
                this->grau_mediana = ((double)(graus[graus.size()/2]) + (double)(graus[(graus.size()/2)-1]))/(double)2;
            }
        }
    
        void escrever_grafo(){
            cout << "Criando graus dos vertices" << endl;
            create_degree();
            cout << "Calculando componentes conexas" << endl;
            componentes_conexas();
            
            cout << "Escrevendo Saida" << endl;
            ofstream file("./" + graph_name + "_saida.txt");
            file << "numero de vértices: " << this->num_vertices - 1 << "\n";
            file << "numero de arestas: " << this->num_arestas << "\n";
            file << "grau mínimo: " << this->grau_min << "\n";
            file << "grau máximo: " << this->grau_max << "\n";
            file << "grau médio: " << this->grau_medio << "\n";
            file << "mediana do grau : " << this->grau_mediana << "\n";
            file << "numero de componentes conexas: " << this->list_comp_conex.size() << "\n";
            file << "tamanho e componentes conexas:" << "\n\t";
            for(vector<int> a : list_comp_conex){
                file << a.size() << ":  ";
                for(int b : a){
                    file << b << " ";
                }
                file << "\n\t";
            }
        }

    
    private:

        vector<vector<int>> arestas;
        // void base_dfs_recursive(int v, vector<bool> &visitado, vector<int> &pai, vector<int> &nivel){
        //     visitado[v] = true;
            // for(int i =0; i < num_vertices; i++){
            //     if(visitado[i] || !arestas[v][i]) continue;
            //     pai[i] = v;
            //     nivel[i] = nivel[v] + 1;
            //     base_dfs_recursive(i, visitado, pai, nivel);
            // }
        // }
        int vizinhos(int i){
            int num = 0;
            for(int j = 1; j < num_vertices; j++){
                if(arestas[i][j]){num++;}
            }
            return num;
        }
};

class graph_list : public graph{
    public:

        graph_list(string a)
        {   
            this->graph_name = a + "";
            read_graph(a);
        }
        
        //le o grafo de um arquivo, seta o número de arestas e arestas
        void read_graph(string filename){

            cout << "Lendo grafo" << endl;
            ifstream file;
            string line;
            file.open(filename, ios::in);

            //seta as informações básicas e gera o gráfico
            if(file.is_open())
            {   

                getline(file, line);
                this->num_vertices = stoi(line.substr(0,line.find_last_of("\n")));
                this->num_vertices++;
                arestas.resize(num_vertices);
                while (getline(file,line))
                {   
                    if(line.size()<1)continue;
                    int n1 = stoi(line.substr(line.find_last_of(" ")+1));
                    int n2 = stoi(line.substr(0, line.find_last_of(" ")));
                    arestas[n1].push_back(n2);
                    arestas[n2].push_back(n1);
                    this->num_arestas++;
                }
                this->num_arestas++;
            }   
            else throw std::invalid_argument( "Impossivel abrir o arquivo" );

        }


        //Realiza busca a partir de um vértice informado, retorna a árvore gerada (por meio do pai)
        //assim como o nível de cada vértice da árvore (raiz é 0). Escrevendo em um arquivo;
        void bfs(int v){
                //roda a bfs
                vector<bool> visitado(num_vertices,false);
                vector<int> pai(num_vertices,-1);
                vector<int> nivel(num_vertices,-1); 
                queue<int> fila;
                fila.push(v);
                visitado[v] = true;
                nivel[v] = 0;
                pai[v] = v;

                while (!fila.empty())
                {
                    int vert = fila.front();
                    fila.pop();
                    for(int a : arestas[vert]){
                        if(visitado[a]) continue;
                        fila.push(a);
                        visitado[a] = true;
                        pai[a] = vert;
                        nivel[a] = nivel[vert] + 1;
                    }
                }

                //escreve os dados gerados em um arquivo
                string a = "./"+graph_name+"_bfs_ " + to_string(v);
                ofstream file(a);
                file << "pai,nivel" << endl;
                for(int i = 1; i < this->num_vertices; i ++){
                    file << to_string(pai[i]) + " " + to_string(nivel[i]) << "\n";
                }

        }



        void dfs_norecursive(int v)
        {
            vector<bool> visitado(this->num_vertices, false);
            vector<int> pai(num_vertices,-1);
            vector<int> nivel(num_vertices,-1);
            stack<int> pilha;

            visitado[v] = true;
            nivel[v] = 0;
            pai[v] = v;
            pilha.push(v);

            while (!pilha.empty()) {
                int vertice = pilha.top();
                pilha.pop();
                if(!visitado[vertice]){
                    visitado[vertice] = true;
                    for (int vizinho : arestas[vertice]) {
                        if (!visitado[vizinho]) {
                            pai[vizinho] = vertice;
                            nivel[vizinho] = nivel[vertice] + 1;
                            pilha.push(vizinho);
                        }
                    }
                }
            }

            //escreve os dados gerados em um arquivo
            string a = "./"+graph_name+"_dfs_ " + to_string(v);
            ofstream file(a);
            file << "pai,nivel\n";
            for(int i = 1; i < this->num_vertices; i ++){
                file << to_string(pai[i]) + " " + to_string(nivel[i]) << "\n";
            }

        }


        //calcula a distancia entre 2 vértices do grafo
        int distancia(int v1, int v2){
            bfs(v1);
            string line;

            string a = "./"+graph_name+"_bfs_ " + to_string(v1);
            ifstream file(a);

            int i =0;
            while (getline(file,line))
            {
                if(i == v2){
                    return stoi(line.substr(line.find_last_of(" ")+1));
                }
                i++;
            }
            return INF;
        }

        
        //calcula o diametro do grafo
        int calc_diametro(){
            
            #pragma omp parallel for
            for(int i = 1; i < num_vertices; i++){
                int di = 0;
                vector<bool> visitado(num_vertices, false);

                queue<pair<int,int>> fila;
                fila.push(make_pair(i,0));
                visitado[i] = true;

                while (!fila.empty())
                {
                    int vert = fila.front().first;
                    int nivel = fila.front().second;
                    fila.pop();

                    for(int a : arestas[vert]){
                        if(visitado[a]) continue;
                        di = max(di, nivel+1);
                        fila.push(make_pair(a,nivel+1));
                        visitado[a] = true;
                    }
                }
                cout << di << endl;

            }
            return this->diametro;
        }

        bool bipartido()
        {
            vector<int> C(this->num_vertices,0);

            for(int i =1; i < this->num_vertices; i++)
                {
                    if(C[i]) continue;
                    queue<int> fila;
                    fila.push(i);
                    C[i] = 1;

                    while (!fila.empty())
                    {
                        int v = fila.front();
                        fila.pop();

                        for(int a : arestas[v])
                        {
                            if(C[a] && C[a] == C[v]) {return false;}
                            if(C[a] && C[a] != C[v]) continue;
                            fila.push(a);
                            C[a] = (C[v]%2)+1;
                        }

                    }
                
                }
            return true;
        }

        
        
        //retorna o numero de componetes, o tamanho de cada e a lista de vértices que participam dele
        void componentes_conexas(){
            vector<bool> visitado(num_vertices, false);
            for(int i = 1; i < num_vertices; i++){
                if(visitado[i])continue;
                this->list_comp_conex.resize(list_comp_conex.size()+1);

                queue<int> fila;
                fila.push(i);
                visitado[i] = true;

                while (!fila.empty())
                {
                    int vert = fila.front();
                    list_comp_conex.back().push_back(vert);
                    fila.pop();
                    for(int a : arestas[vert]){
                        if(visitado[a]) continue;
                        fila.push(a);
                        visitado[a] = true;
                    }
                }

            }
            sort(list_comp_conex.rbegin(), list_comp_conex.rend());

        }

        
        //gera grau min, grau max, grau medio e mediana do grau
        void create_degree(){
            
            vector<int> graus;
            for(int i = 1; i < num_vertices; i++){
                graus.push_back(this->vizinhos(i));
            }
            sort(graus.begin(), graus.end());
            this->grau_max = graus.back();
            this->grau_min = graus.front();
            this->grau_medio = (double)2 * ((double)this->num_arestas/(double)(this->num_vertices-1));
            if(graus.size()%2 == 1){
                this->grau_mediana = graus[graus.size()/2];
            }
            else{
                this->grau_mediana = ((double)(graus[graus.size()/2]) + (double)(graus[(graus.size()/2)-1]))/(double)2;
            }
        }
    
        void escrever_grafo(){
            create_degree();
            componentes_conexas();
            

            ofstream file("./"+graph_name+"_saida.txt");
            file << "numero de vértices: " << this->num_vertices - 1 << "\n";
            file << "numero de arestas: " << this->num_arestas << "\n";
            file << "grau mínimo: " << this->grau_min << "\n";
            file << "grau máximo: " << this->grau_max << "\n";
            file << "grau médio: " << this->grau_medio << "\n";
            file << "mediana do grau: " << this->grau_mediana << "\n";
            file << "é bipartido: " << bipartido() << "\n";
            file << "numero de componentes conexas: " << this->list_comp_conex.size() << "\n";
            file << "tamanho e componentes conexas:" << "\n\t";
            for(vector<int> a : list_comp_conex){
                file << a.size() << ":  ";
                for(int b : a){
                    file << b << " ";
                }
                file << "\n\t";
            }
        }
    
    private:

        vector<list<int>> arestas;

        int vizinhos(int i){
            return arestas[i].size();
        }
};

int main()
{
    cout << "INCIANDO PROGRAMA" << "\n";
    for(int i =2; i <= 6; i++)
    {   
        graph_vetor g1("./grafos/grafo_"+to_string(i)+".txt");
        // cout << "Rodando grafo " << i << "\n";

        //Ex 2
        double sum =0;
        cout << "RODANDO BFS's" << endl;
        for(int j = 1; j <= 100; j ++)
        {
            clock_t start_time = clock();
            g1.bfs(j);
            clock_t end_time = clock();
            ofstream file;
            file.open("BFS_TIME_G" + to_string(i) + ".txt", ios_base::app);
            double elapsed = (end_time - start_time)/(double)CLOCKS_PER_SEC;
            file << elapsed << "\n";
            sum += elapsed;
            if(j==100){file << "média: "<< (double)sum/(double)100 << "\n";}
            file.close();
        }
        
        // //Ex3
        sum = 0;
        cout << "RODANDO DFS's" << endl;
        for(int j = 1; j <= 100; j ++)
        {
            clock_t start_time = clock();
            g1.dfs_norecursive(j * 5);
            clock_t end_time = clock();
            ofstream file;
            file.open("DFS_TIME_G" + to_string(i) + ".txt", ios_base::app);
            double elapsed = (end_time - start_time) / (double)CLOCKS_PER_SEC;
            file << elapsed << "\n";
            sum += elapsed;
            if(j==100){file << "média: " << (double)sum/(double)100 << "\n";}
            file.close();
        }


        // //Ex5
        ofstream file;
        cout << "Calculando distancias" << endl;
        file.open("Distancias" + to_string(i) + ".txt", ios_base::app);
        file << "10-20: " << g1.distancia(10,20) << "\n";
        file << "10-30: " << g1.distancia(10,30) << "\n";
        file << "20-30: " << g1.distancia(20,30) << "\n";
        file.close();


        // //Ex6 quantas componentes conexas
        g1.escrever_grafo();
        #pragma omp parallel
         {
            int thread_id = omp_get_thread_num();
            cout << "Hello from thread " << thread_id << endl;
        }

        //Ex7 diametro
        cout << "Calculando diametro" << endl;
        file.open("Diametro" + to_string(i) + ".txt", ios_base::app);
        clock_t start_time = clock();
        file << g1.calc_diametro() << "\n";
        clock_t end_time = clock();
        double elapsed = (end_time - start_time) / (double)CLOCKS_PER_SEC;
        file << elapsed << "\n";
        file.close();

    }

        // //Codigo usado para descobrir a memória utilizada
        // PROCESS_MEMORY_COUNTERS_EX pmc;
        // GetProcessMemoryInfo(GetCurrentProcess(), (PROCESS_MEMORY_COUNTERS*)&pmc, sizeof(pmc));
        // //virtual memory currently used by current process
        // SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
        // //Physical Memory currently used by current process
        // SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
        // cout << "memoria usada: "<< physMemUsedByMe << endl;


}
