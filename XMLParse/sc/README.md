# XMLPARSER:

XMLParser is a C program allowing us to parse a XML file from BLAST. 

To compile it : 
- make

To execute it : 
- ./XMLParser [filename]

The program returns a CSV file (result.csv) containing : 
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
