# test
import json
import matplotlib.pyplot as plt

with open('ARL_gold.json', 'r') as f:
    data = json.load(f)

lengths = []
iterations = []
for i, hit in enumerate(data['blast_output'][0]['hits']):
    length = int(hit['align-len'])
    lengths.append(length)
    iterations.append(i)
   
# échelonner l'axe des abscisse de 500 à 500
plt.xticks(range(0, 5000, 500))
# afficher des points
plt.scatter(iterations, lengths, s=1)
plt.xlabel('Hits')
plt.ylabel('Lengths')
# tracer une droite de valeurs lengths[0]
plt.axhline(lengths[0], color='r', linestyle='--')
# afficher sur le graphe la valeur de lengths[0] 
plt.text(i-10, lengths[0]+1, lengths[0])
# Ajouter un titre
plt.title('Histogram of lengths')
# enregistrer le graphe
plt.savefig('Distribution_longueur_ARL6.jpg')
# afficher le graphe
plt.show()
