import json
import matplotlib.pyplot as plt

with open('../XMLParse/newVersion/ARL6.json', 'r') as f:
    data = json.load(f)
    
# Extraire les longueurs des hits
lengths = [int(hit["align-len"]) for hit in data["blast_output"][0]["hits"]]

# Créer une liste de même longueur que 'lengths' contenant l'itération actuelle
iteration = list(range(1, len(lengths) + 1))

# Créer un diagramme en boîte à moustache avec les longueurs en abscisse et l'itération en ordonnée
plt.boxplot(lengths)
plt.xticks([1], [min(lengths)])
plt.xlabel("Longueur")
plt.ylabel("Itération")
plt.title("Distribution des longueurs")
plt.plot(lengths, iteration, 'o', alpha=0.3)
plt.savefig('Distribution_longueur_ARL6.jpg')
plt.show()



# Extraire les longueurs pour chaque hit accession
# lengths = []
# for blast_output in data['blast_output']:
#     for hit in blast_output['hits']:
#         length = int(hit['align-len'])
#         lengths.append(length)

# # Créer un dictionnaire pour stocker les itérations de longueur
# length_iters = {}
# iter_num = 1
# for length in sorted(set(lengths)):
#     length_iters[length] = iter_num
#     iter_num += 1

# # Créer un tableau d'itérations de longueur pour chaque hit accession
# hit_iters = []
# for blast_output in data['blast_output']:
#     for hit in blast_output['hits']:
#         length = int(hit['align-len'])
#         hit_iters.append(length_iters[length])

# # Tracer la boîte à moustache
# plt.boxplot(hit_iters, vert=False)

# # Définir les étiquettes d'axe
# plt.xlabel('Longueurs')
# plt.ylabel('Itérations')
# plt.savefig('Distribution_longueur_ARL6.jpg')
# plt.show()

