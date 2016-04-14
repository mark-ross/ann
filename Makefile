UNAME := $(shell uname -s)
ifeq ($(UNAME),Darwin)
	CXX = clang++
	FLAGS = -pthread -std=c++11 -stdlib=libc++
else
	CXX = g++
	FLAGS = -pthread -std=c++11
endif



ann: neuralNetwork.o main.o
	$(CXX) -o ann main.o neuralNetwork.o $(FLAGS)

neuralNetwork.o: neuralNetwork.h neuralNetwork.cpp
	$(CXX) -c -g neuralNetwork.cpp $(FLAGS)

main.o: main.cpp
	$(CXX) -c -g main.cpp $(FLAGS)

clean:
	rm -f *~ ann *.o
