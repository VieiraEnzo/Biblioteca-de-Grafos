import matplotlib.pyplot as plt

# Supondo que você tenha os tempos de execução para cada algoritmo e grafo
tempos_execucao_algo1 = [0.100911, 0.27937, 0.358631, 11.2173, 18.355, 15.5863]
tempos_execucao_algo2 = [0.124833, 0.382589, 0.402658, 9.66615, 17.4481, 10.1739]
tempos_execucao_algo3 = [0.0343344, 0.0622428, 0.281344, 7.50717, 16.8695, 13.71]

grafos = [1, 2, 3, 4, 5, 6]

plt.plot(grafos, tempos_execucao_algo1, label='Ford Fulkerson')
plt.plot(grafos, tempos_execucao_algo2, label='Ford Fulkerson (map)')
plt.plot(grafos, tempos_execucao_algo3, label='Dinitz')

plt.xlabel('Grafos')
plt.ylabel('Tempo de Execução')
plt.title('Tempo de Execução dos Algoritmos')
plt.legend()
plt.show()
