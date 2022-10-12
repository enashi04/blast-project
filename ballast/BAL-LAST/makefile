# L'exécutable à générer :

EXEC=ballast

# Les fichiers .o a générer si nécessaire

DEP=printusage.o getargs.o initresfile.o addprofils.o handlegaps.o sortbybegdb.o mhalign.o consmotif.o freesbjmotifs.o getsbjmotifs.o filter.o profilBuilding.o substring.o readmask.o readprofil.o maskprofil.o smoothprofil.o consseq.o trimprofil.o trimprofilext.o extractmotifs.o profilplot.o printnicedouble.o countmatches.o overlap.o scalaire.o printmsf.o printheader.o printstats.o printqueryseq.o sortbysc.o redundout.o riseup.o printresout.o showmotifs.o gcgcheck.o

all: $(EXEC)


#Variables personnalisées

CC=gcc

CFLAGS=-Wall -g

LDFLAGS=



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



