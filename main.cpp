#include <cstdlib>
#include <iostream>

using namespace std;

#include "neuralNetwork.h"

int main(int argc, char* argv[]) {
    
    neuralNetwork q;
    
    if(argc < 2) {
        q.run(0);  //without error stuff
    } else {
        q.run(1);  //with error calculation
    }
    
    return 0;
}