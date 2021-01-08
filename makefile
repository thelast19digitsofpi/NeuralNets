CC=g++
CXXFLAGS=-std=c++11 -g -Wall
OFILES=main.o util.o Network.o SoftmaxLayer.o SigmoidLayer.o OutputLayer.o InputLayer.o NeuralError.o

neuralNets: $(OFILES)
	$(CC) $(CXXFLAGS) $(OFILES) -o neuralNets

main.o: main.cpp OutputLayer.h InputLayer.h Network.h

Network.o: Network.cpp Network.h InputLayer.h OutputLayer.h

SoftmaxLayer.o: SoftmaxLayer.cpp SoftmaxLayer.h OutputLayer.h InputLayer.h

SigmoidLayer.o: SigmoidLayer.cpp SigmoidLayer.h OutputLayer.h InputLayer.h

OutputLayer.o: OutputLayer.cpp OutputLayer.h InputLayer.h

InputLayer.o: InputLayer.cpp InputLayer.h

NeuralError.o: NeuralError.cpp NeuralError.h

util.o: util.cpp util.h


run: neuralNets
	./neuralNets

clean:
	rm -f *.o
	rm -f neuralNets

valgrind: neuralNets
	valgrind ./neuralNets

gdb: neuralNets
	gdb ./neuralNets
