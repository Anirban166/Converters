C = gcc
FLAGS = -Wall -g

exec: converter.o ppmP3.o ppmP6.o pamP7.o utilityFunctions.o
	$(C) $(FLAGS) -o exec converter.o ppmP3.o ppmP6.o pamP7.o utilityFunctions.o

converter.o: converter.c
	$(C) $(FLAGS) -c converter.c

ppmP3.o: ppmP3.c
	$(C) $(FLAGS) -c ppmP3.c

ppmP6.o: ppmP6.c
	$(C) $(FLAGS) -c ppmP6.c

pamP7.o: pamP7.c
	$(C) $(FLAGS) -c pamP7.c

utilityFunctions.o: utilityFunctions.c
	$(C) $(FLAGS) -c utilityFunctions.c

clean:
	rm exec converter.o ppmP3.o ppmP6.o pamP7.o utilityFunctions.o	