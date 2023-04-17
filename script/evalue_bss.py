# importation json
import json
import matplotlib.pyplot as plt

# ouvrir le fichier json
with open('bbs.json', 'r') as f:
    data = json.load(f)

# initialisation de la variable evalue
evalues = []
ranks = []

# récupérer les evalues pour chaque hit
for i,hit in enumerate(data['blast_output']):
    query_name =hit["query-name"].split("|")[2].split()[0]
    for i,data in enumerate(hit['hits']):
        evalue = float(data['align-len'])
        evalues.append(evalue)
        ranks.append(i)

# tracer le graphe avec les evalues
plt.plot(evalues)
plt.xlabel('Ranks')
plt.ylabel('length')
plt.title('Distribution of length of BBSOME')
plt.savefig('test.jpg')
plt.show()
plt.clf()  # clear current figure to avoid plotting everything on top of each other
