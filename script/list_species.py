# import json

# # Ouvrir le fichier JSON et charger son contenu dans un dictionnaire Python
# with open('../XMLParse/newVersion/ARL6.json', 'r') as f:
#     data = json.load(f)


# for hit in data['blast_output'][0]['hits']:
#     try:
#         species_name = hit['species']['name']
#         print(species_name)
#     except KeyError:
#         print(f"La clé 'name' n'est pas présente pour cet élément : {hit}")

import json
import matplotlib.pyplot as plt

with open('../XMLParse/newVersion/ARL6.json', 'r') as f:
    data = json.load(f)

# Créer un dictionnaire pour compter le nombre d'occurrences de chaque espèce
species_counts = {}
for hit in data['blast_output'][0]['hits']:
    try :
        species_name = hit['species']['name']
        if species_name in species_counts:
            species_counts[species_name] += 1
        else:
            species_counts[species_name] = 1
    except KeyError:
        pass

# Créer un graphique en barres pour représenter les données
plt.bar(range(len(species_counts)), list(species_counts.values()))
plt.xticks(range(len(species_counts)), list(species_counts.keys()), rotation=90)
plt.title('Nombre d\'occurrences de chaque espèce')
plt.xlabel('Espèces')
plt.ylabel('Nombre d\'occurrences')
plt.savefig('Species_ARL6.jpg')
plt.show()
