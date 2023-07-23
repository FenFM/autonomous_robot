comm:	communicator.o network.o
		gcc -lm -c communicator.o network.o -o comm

communicator.o:	communicator.c
				gcc -c communicator.c

splitted_network.o:	network.c
					gcc -c network.c

clean:
	rm -f *.o comm