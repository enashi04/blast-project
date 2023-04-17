# import json
# import matplotlib.pyplot as plt

# # ouvrir le fichier json
# with open('bbs.json', 'r') as f:
#     data = json.load(f)

# j=1
# for i,hit in enumerate(data['blast_output']):
#     query_name = hit["query-name"]
#     gene_name = query_name.split("|")[2].split()[0]
#   # récupérer les evalues pour chaque hit
#     evalues = []
#     ranks=[]
#     for i,data in enumerate(hit['hits']):
#         evalue = float(data['evalue'])
#         evalues.append(evalue)
#         ranks.append(i)

import json
import matplotlib.pyplot as plt
import seaborn as sns

# ouvrir le fichier json
with open('bbs.json', 'r') as f:
    data = json.load(f)

# initialiser une liste pour stocker les evalues pour chaque hit
evalues_list = []
query_names=[]
# parcourir le fichier et récupérer les evalues pour chaque hit
for hit in data['blast_output']:
    query_name = hit['query-name']
    gene_name =query_name.split("|")[2].split()[0]
    query_names.append(gene_name)
    evalues = []
    for data in hit['hits']:
        evalue = float(data['align-len'])
        evalues.append(evalue)
    # ajouter les evalues à la liste
    evalues_list.append(evalues)

# créer le violin plot
fig, ax = plt.subplots()
ax.violinplot(evalues_list)
ax.set_xticks(range(1, len(query_names) + 1))
ax.set_xticklabels(query_names, rotation=25, fontsize=6)
ax.set_ylabel('Length')
ax.set_title('Distribution of length')
plt.savefig('length.png')
plt.show()

    # tracer le graphe avec les evalues
   # échelle logarithmique sur l'axe des y
    # plt.yscale('log')
    # # échelonner l'axe des abscisse de 500 à 500
    # plt.xticks(range(0, 5000, 500))
    # # afficher des points
    # plt.scatter(ranks, evalues, s=1)
    # plt.xlabel('Ranks')
    # plt.ylabel('Evalue')
    # plt.title('Distribution of evalues of '+gene_name, fontsize=8)
    # # plt.text(len(evalues) - 1, evalues[-1], query_name, ha='right', va='center', fontsize=8)
    # plt.savefig('Distribution_evalues_BBSOME'+str(j)+'.jpg')
    # plt.show()
    # plt.clf()  # clear current figure to avoid plotting everything on top of each other

    # j+=1





