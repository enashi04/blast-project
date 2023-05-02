# BLAST-PROJECT

## **Member :**
- Ihsane ERRAMI 

The objective of this project is to develop a program to enrich blast data and facilitate bioinformaticians to process them.

## **❗ Important**

To be able to compile this program, you have to be on Linux or MacOS operating system. 

* If you are on Window, you can install WSL by following this [tutorial](https://learn.microsoft.com/en-us/windows/wsl/install).
* You need the taxonomy.dat file.

> The libxml library and unicode library are included in the project. 

## Summary 

The blast-project contains two programs : 
- BALLAST : Blast Post-Processing based on locally conserved segments.

  This program was developed in 2000's by Julie Thompson, Olivier Poch and Frederic Plewniak in C language. The goal is to  predict local maximum segments from a single Blast database search and to highlight these divergent homologues.

  More informations about Ballast [here](https://academic.oup.com/bioinformatics/article/16/9/750/307628)

- BRISTOOL :  Blast enrichment analysis tool 
  
  This program was developed in order to answer some problem :
  
  The problem is that there is an increasing growth of data in the database, so there are very considerable exploitation elements for the researchers.
  You can have more than 20,000 target sequences in a blast.
  
  The goal is to find a way to facilitate the exploitation of this data. 
  
  BRISTOOL is a program that is coded in C, using a librarian: libxml to browse the XML file. 
  
  The aim of this program is to extract the data and to add others and put them in JSON format.
  
  


  
