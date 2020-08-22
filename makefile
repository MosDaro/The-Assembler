assembler: assembler.o fileHandler.o parse.o symbolHandler.o errors.o dataStructs.o instruction.o
	gcc -g3 -Wall -ansi -pedantic \
	assembler.o fileHandler.o parse.o symbolHandler.o errors.o dataStructs.o instruction.o \
	-o assembler

assembler.o: assembler.c
	gcc -c -g3 -Wall -ansi -pedantic assembler.c -o assembler.o

fileHandler.o: fileHandler.c fileHandler.h parse.h symbolHandler.h dataStructs.h global.h
	gcc -c -g3 -Wall -ansi -pedantic fileHandler.c -o fileHandler.o

parse.o: parse.c parse.h errors.h dataStructs.h instruction.h
	gcc -c -g3 -Wall -ansi -pedantic parse.c -o parse.o

symbolHandler.o: symbolHandler.c symbolHandler.h errors.h
	gcc -c -g3 -Wall -ansi -pedantic symbolHandler.c -o symbolHandler.o

errors.o: errors.c errors.h global.h
	gcc -c -g3 -Wall -ansi -pedantic errors.c -o errors.o

dataStructs.o: dataStructs.c dataStructs.h symbolHandler.h
	gcc -c -g3 -Wall -ansi -pedantic dataStructs.c -o dataStructs.o

instruction.o: instruction.c instruction.h errors.h symbolHandler.h dataStructs.h
	gcc -c -g3 -Wall -ansi -pedantic instruction.c -o instruction.o

clean :
	rm -rf *.o *.ent *.ext *.ob $(objects) assembler