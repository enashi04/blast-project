# XMLPARSER:

XMLParser is a C program allowing us to parse a XML file from BLAST. 

There are two versions of XMLParser:
- with the libxml library (version 1)
- with the expat library (version 2)

Both versions work with.

To compile it : 
- make

To execute it : 
- ./parsing [filename] [format]

filename is the name of XML file with '.xml" extension

format is a number between 1 -3 allowing to return a CSV file containing:

- 1 : Bronze format
    - id  of the hit, 
    - number of the hit, 
    - score, 
    - evalue,
    - and align-length 

- 2 : Silver format
    - id  of the hit, 
    - number of the hit, 
    - score, 
    - evalue,
    - identity
    - positive
    - gaps
    - and align-length 
- 3 : Gold format
    - the query and its length
    - hit id ,
    - bit-score,
    - score,
    - evalue,
    - query-from,
    - query-to,
    - target-from,
    - target-to,
    - identity,
    - positive,
    - gaps,
    - align-length

The results are in the results folder.
