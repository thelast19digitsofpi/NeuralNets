CC=g++
CFLAGS=-std=c++11 -g -Wall
OFILES=main.o util.o OutputLayer.o InputLayer.o

neuralNets: $(OFILES)
	$(CC) $(FLAGS) $(OFILES) -o neuralNets

main.o: main.cpp OutputLayer.h InputLayer.h Network.h
	$(CC) $(FLAGS) -c main.cpp

OutputLayer.o: OutputLayer.cpp OutputLayer.h InputLayer.h
	$(CC) $(CFLAGS) -c OutputLayer.cpp

InputLayer.o: InputLayer.cpp InputLayer.h
	$(CC) $(CFLAGS) -c InputLayer.cpp

util.o: util.cpp util.h
	$(CC) $(CFLAGS) -c util.cpp


run: neuralNets
	./neuralNets

clean:
	rm -f *.o
	rm -f neuralNets

valgrind: neuralNets
	valgrind ./neuralNets

