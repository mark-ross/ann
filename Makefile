ann: neuralNetwork.o main.o
	g++ -o ann main.o neuralNetwork.o

neuralNetwork.o: neuralNetwork.h neuralNetwork.cpp
	g++ -c -g neuralNetwork.cpp -std=c++11

main.o: main.cpp
	g++ -c -g main.cpp

clean:
	rm -f *~ ann *.o
