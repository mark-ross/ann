#include <cstdlib>
#include <iostream>

using namespace std;

#include "neuralNetwork.h"

int main(int argc, char* argv[]) {
    
    neuralNetwork q(argc,argv);
    
    q.run();
    
    return 0;
}