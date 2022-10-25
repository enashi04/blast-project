
cazy (EC1....)=> enzyme
Ig et système MHC 
kinases (ST dualspecifity Thréonine)
TF
-> but : groupe d'orthologie 
utiliser orthoinspector afin de vérifier les motifs des kinases
PFAM (CDD): banque de domaine but ps de notions d'orthologie car transgressive (=> aller chez tout le monde)
Bestites réciproques ? dès le 1e blat on a un groupe d'orthologue
=> nos motifs retrouvent ils les motifs (qui permet d'anonter les genomes).
possibilité de comparer avec la dérive du psyblast  (protéine biaisé so complicated d'avoir un bon résulat car motifs nulle donc dérive bof)

>> trouver qqch de + robuste et permettre d'avoir un redépôt du piepalign.
le coupler à prob -> faire en logique web ! 

Infrastructure des banques 
=> score pour les clusters (Kirsley) qui est géré en amont 
=> algo optimisation (Pierre C.)
=> à un moment faut tester les 20k de protéines (humaine/plante et bactérie + archées)=> pour rencontrer tous les cas de figures
=> évaluation des erreurs dans les primates => donner des orthologues et celles qui ne sont pas modèle, proche en automatique.
=> aller à 10^-5
=> probleme de score à vérifier ! urgent car bcp de fragments 
=> protéine mal prédite fréquemment à trouver, de faible complexité (avec bcp de régions) les **préproprotéines** qui sont mal prédites

                                   _______________________


# Reunion 5 : 

Le code de Ballast a l'air bon. (Vérifier dans pipealign des targets supprimés car doublon /isoformes ...)

Concernant la suite du projet : savoir si ca rentre dans les compétences de l'apprentissage.

* réaliser un jeu de test:  
    * par exemple pour un orthologue quelle query prendre? 
    * pour une query, trouver son orthologue
    * à partir d'un motif précis trouver son orthologue


* ballast doit donner une sorte de score dans l'orthologie.

_On se dirige vers de l'IA ambitieuse, de la recherche?_  
* Faire un blast pour récupérer le vrai orthologue

* Interface Web pour y ajouter Ballast :
    * le but est de coupler certaines plateformes déjà réalisées et le mettre sous une plateforme.
* upgrade le pipealign avec la nouvelle version de Ballast.



