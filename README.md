# Assembler
 __Program Description__: The program Assembler gets from the user files' names without a suffix in the command line that contains assembly code.
 *  The Assembler runs on each file and checks the syntax. 

 *  The Assembler translates each line from the given files to Hexadecimal code which is the Machine code of each line.
 
 *  When the file is translated successfully the Assembler creates 3 types of out files:
    * "input file name".ob = file that contains all the addresses and the Machine code value of each line from the given file.
    * "input file name".ent = file that contains all the entry symbols in the given input file and their addresses.
    * "input file name".ext = file that contains all the external symbols in the given input file and their addresses.
 *  The Assembler uses 2 passes:
    * In the first pass, the Assembler creates a symbol table and a structure of data and code memory picture and maintains it during the run.
    * In the second pass, the Assembler fills the missing values to complete the memory picture, and then creates output files.
