import json
import matplotlib.pyplot as plt

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
                if species_name.lower()=="fungi" or species_name.lower()=="viridiplantae" or species_name.lower()=="primates" or species_name.lower()=="mammalia" or species_name.lower()=="arthropoda" or species_name.lower()=="nematoda" or species_name.lower()=="insecta":
                    species[species_name] = species.get(species_name, 0) + 1
                # else :
                #     species[""] = species.get("", 0) + 1

        except KeyError:
            pass
    # trier les espèces pour chaque query
    sorted_species = sorted(species.items(), key=lambda x: x[1], reverse=True)
    # ajouter les espèces pour chaque query à la liste de dictionnaires
    species_list.append(sorted_species)

# créer une liste pour stocker les noms de chaque espèce
species_names = []
for i in range(len(species_list)):
    series = species_list[i]
    names = []
    for j in range(len(series)):
        names.append(series[j][0])
    species_names.append(names)

# créer une liste pour stocker les valeurs de chaque série
series_values = []
for i in range(len(species_list)):
    series = species_list[i]
    occurrences = []
    for j in range(len(series)):
        occurrences.append(series[j][1])
    series_values.append(occurrences)

# créer une liste pour stocker les couleurs de chaque série
colors=['yellow', 'green', 'grey', 'blue', 'orange', 'purple','red'][:len(series_values)]

# créer une liste pour stocker les positions de chaque nom d'espèce sur l'axe des x
species_positions = [i+1 for i in range(len(species_names[0]))]

# tracer l'histogramme avec les deux séries
plt.hist(series_values, bins=species_positions, color=colors, edgecolor='red', hatch='/', label=['Query 1', 'Query 2'], histtype='bar')

# mettre les noms des espèces sur l'axe des x
plt.xticks(species_positions, species_names[0], rotation=20, ha='right')

plt.ylabel('Number of species')
plt.xlabel('Species')
plt.title('Species distribution per query')
plt.legend()
plt.savefig('histogram.png')
plt.show()
