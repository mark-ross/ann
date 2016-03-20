ann: neuralNetwork.o main.o node.o
	g++ -g -o ann main.o neuralNetwork.o node.o

neuralNetwork.o: neuralNetwork.h neuralNetwork.cpp
	g++ -c -g neuralNetwork.cpp

node.o:	node.h node.cpp
	g++ -c -g node.cpp

main.o: main.cpp
	g++ -c -g main.cpp

clean:
	rm -f *~ ann *.o
