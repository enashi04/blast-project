import json
import numpy as np
import matplotlib.pyplot as plt

# ouvrir le fichier json
with open('bbs.json', 'r') as f:
    data = json.load(f)

# initialiser une liste pour stocker les longueurs des espèces pour chaque query
species={}

# parcourir le fichier et récupérer le nom de l'espece et sa longueur pour chaque query
species[' ']='0'
for hit in data['blast_output']:
    query_name = hit["query-name"]
    gene_name = query_name.split("|")[2].split()[0]
    length = hit["query-length"]
    print(length)
    # ajouter la longueur de l'espèce à la liste    
    species[gene_name]=length

species['BBS10_HUMAN']='723'
# tracer l'histogramme
for key in species:
    plt.bar(key, species[key], color='orange', width=0.5)
    plt.xticks(fontsize=7,rotation=25)

plt.ylabel('Query length')
plt.xlabel('Query Name')
plt.title('Query length distribution of BBSOME')
plt.savefig('histogram1.png')
plt.show()