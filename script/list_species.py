import json
import matplotlib.pyplot as plt

with open("ARL_gold.json") as f:
    data = json.load(f)

species={}
# parcourir la liste des espèces
for i, hit in enumerate(data['blast_output'][0]['hits']):
    # récupérer la liste des espèces
    try :
        lineage = hit['species']['lineage']
        if len(lineage) >= 10:
            species_name = lineage[9]['name']
            species[species_name] = species.get(species_name, 0) + 1
        else:
            species_name = 'others'
            species[species_name] = species.get(species_name, 0) + 1
    except KeyError:
        pass

sorted_species = sorted(species.items(), key=lambda x: x[1], reverse=True)

# afficher les espèces triées dans un tableau
print("{:<20} {:<10}".format("Espèce", "Occurrences"))
for s in sorted_species:
    print("{:<20} {:<10}".format(s[0], s[1]))
