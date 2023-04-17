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

    # afficher les espèces triées pour chaque query dans un tableau
    total_occurrences = sum(species.values())
    print("Query: "+gene_name)
    print("{:<20};{:<10};{:<10}".format("Espèce", "Occurrences", "Pourcentage"))
    for s in sorted_species:
        species_name = s[0]
        occurrences = s[1]
        percentage = (occurrences / total_occurrences) * 100
        print("{:<20};{:<10};{:<10.2f}%".format(species_name, occurrences, percentage))
    print("\n")
# colors=['green', 'grey', 'blue', 'yellow', 'red', 'orange', 'purple','red']
# for i in range(len(species_list)):
#     if(i==0 or i ==2):
#         species = species_list[i]

#         species_name = []
#         occurrences = []
#         for j in range(len(species)):
#             species_name.append(species[j][0])
#             occurrences.append(species[j][1])
            
#         plt.bar(species_name, occurrences, label = "Query "+str(i+1), color=colors[i%len(colors)])
#         plt.xticks(rotation=25, fontsize=7)
# plt.ylabel('Number of species')

# plt.legend()
# plt.savefig('testtest.png')
# plt.show()