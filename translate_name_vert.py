import csv
verts =  [343930,11466,68773,4379,371226,391667,7200,2722]
trans = {}

with open("grafos_tp2/rede_colaboracao_vertices.txt", 'r',  encoding='utf-8') as file:
    file.readline()
    # Create a CSV reader
    csv_reader = csv.reader(file)
    
    # Iterate through each row in the CSV file
    for row in csv_reader:
        # Each row is a list of values
        # Access the values by their index
        trans[int(row[0])] = row[1]

separator = "->"
for vert in verts:
    print(trans[vert], end=separator)