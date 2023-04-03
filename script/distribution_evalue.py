import json
import matplotlib.pyplot as plt

with open('../XMLParse/newVersion/ARL6.json', 'r') as f:
    data = json.load(f)

# Récupérer les evalues et les stocker dans une liste
evalues = []
for hit in data['blast_output'][0]['hits']:
    evalue = float(hit['evalue'])
    evalues.append(evalue)

# Créer un nuage de points des evalues avec le numéro de hit en ordonnée
plt.scatter(evalues, range(1, len(evalues)))
plt.xlabel('evalue')
plt.ylabel('Numéro de hit')
plt.title('Distribution des evalues')
plt.savefig('Distribution_evalues_ARL6.jpg')
plt.show()

