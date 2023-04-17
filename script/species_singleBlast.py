import json
import matplotlib.pyplot as plt
import numpy as np
import random

# ouvrir le fichier json
with open('bbs.json', 'r') as f:
    data = json.load(f)
# initialiser une liste de dictionnaires pour stocker les espèces pour chaque query
species_list = []

# parcourir le fichier et récupérer le nom de l'espece pour chaque query
for hit in data['blast_output']:
    species={}
    query_name = hit["query-name"]
    gene_name = query_name.split("|")[2].split()[0]
    # parcourir la liste des espèces pour chaque query
    for i,data in enumerate(hit['hits']):
        try:
            lineage = data['species']['lineage']
            for j in range(len(lineage)):
                species_name = lineage[j]['name']
                if species_name.lower()=="mammalia" or species_name.lower()=="fungi" or species_name.lower()=="viridiplantae" or species_name.lower()=="primates" :
                    species[species_name] = species.get(species_name, 0) + 1
                else :
                    species["others"] = species.get(species_name, 0) + 1

        except KeyError:
            pass
     # trier les espèces pour chaque query
    sorted_species = sorted(species.items(), key=lambda x: x[1], reverse=True)
    # ajouter les espèces pour chaque query à la liste de dictionnaires
    species_list.append(sorted_species)

colors=['green', 'grey', 'blue', 'yellow', 'black']
k=1;
for i in range(len(species_list)):
    species = species_list[i]
    for j in range(len(species)):
        plt.bar(species[j][0],species[j][1], color=colors[j])

plt.savefig("speciesARL.png")
plt.show()



