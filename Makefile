ann: neuralNetwork.o main.o
	g++ -o ann main.o neuralNetwork.o -pthread -std=c++11

neuralNetwork.o: neuralNetwork.h neuralNetwork.cpp
	g++ -c -g neuralNetwork.cpp -pthread -std=c++11

main.o: main.cpp
	g++ -c -g main.cpp -pthread -std=c++11

clean:
	rm -f *~ ann *.o
