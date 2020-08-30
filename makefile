assembler: assembler.o fileHandler.o parse.o symbolHandler.o errors.o dataStructs.o instruction.o
	gcc -g -Wall -ansi -pedantic \
	assembler.o fileHandler.o parse.o symbolHandler.o errors.o dataStructs.o instruction.o \
	-o assembler

assembler.o: assembler.c fileHandler.h
	gcc -c -g -Wall -ansi -pedantic assembler.c -o assembler.o

fileHandler.o: fileHandler.c fileHandler.h parse.h global.h
	gcc -c -g -Wall -ansi -pedantic fileHandler.c -o fileHandler.o

parse.o: parse.c parse.h symbolHandler.h
	gcc -c -g -Wall -ansi -pedantic parse.c -o parse.o

symbolHandler.o: symbolHandler.c symbolHandler.h instruction.h
	gcc -c -g -Wall -ansi -pedantic symbolHandler.c -o symbolHandler.o

errors.o: errors.c global.h
	gcc -c -g -Wall -ansi -pedantic errors.c -o errors.o

dataStructs.o: dataStructs.c dataStructs.h global.h
	gcc -c -g -Wall -ansi -pedantic dataStructs.c -o dataStructs.o

instruction.o: instruction.c instruction.h parse.h dataStructs.h
	gcc -c -g -Wall -ansi -pedantic instruction.c -o instruction.o

clean :
	rm -rf *.o *.ent *.ext *.ob $(objects) assembler