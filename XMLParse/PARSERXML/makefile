# L'exécutable à générer :
EXEC=parsing

# Les fichiers .o a générer si nécessaire
DEP=options.o converttoJSON.o readTaxonomy.o

all: $(EXEC)

#Variables personnalisées
CC=gcc
CFLAGS=-Wall -g
LDFLAGS= -lexpat


%.o: %.c 
	$(CC) -c $^ $(CFLAGS)

#Générer les commandes demandées une après l'autre
$(EXEC): %: %.c $(DEP)
	$(CC) $(CFLAGS) -o $@  $^ $(LDFLAGS)

#### RAPPEL ################

#   $@    Nom de la cible
#   $<     Nom de la première dépendance pour la cible
#   $ˆ      Liste des dépendances pour la cible
#   $?     Liste des dépendances plus récentes que la cible Nom du fichier (sans son suffixe)
#   $*      Nom du fichier (sans son suffixe)
#    % : permet de prendre un élément après l'autre dans une liste de cibles
##################

#supprimer les fichier *.o 
clean:
	rm -rf *.o 

#supprimer les fichiers *.o et les exécutables
mrproper: clean
	rm -rf $(EXEC) 
