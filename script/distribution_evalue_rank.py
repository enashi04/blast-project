import json
import matplotlib.pyplot as plt

# ouvrir le fichier json
with open('bbs.json', 'r') as f:
    data = json.load(f)

# récupérer les evalue
evalues = []
ranks= []
for i,hit in enumerate(data['blast_output'][0]['hits']):
    evalue = float(hit['evalue'])
    evalues.append(evalue)
    ranks.append(i)

# échelle logarithmique sur l'axe des y
plt.yscale('log')
# échelonner l'axe des abscisse de 500 à 500
plt.xticks(range(0, i, 500))
# afficher des points
plt.scatter(ranks, evalues, s=1)
plt.xlabel('Ranks')
plt.ylabel('Evalue')
# afficher sur le graphe la valeur de lengths[0] 
plt.text(-1100, evalues[0], evalues[0], color='r')
# Ajouter un titre
plt.title('Distribution of evalues')
plt.savefig('Distribution_evalues_BBS.jpg')
plt.show()