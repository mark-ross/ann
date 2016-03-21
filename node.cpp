#include "node.h"
#include <cstdlib>
#include <iostream>

using namespace std;


node::node() {
    //There shouldn't be any weights here
    weights = 0;
    numWeights = 0;
    //set the value to the input
    value = 0;
}

node::node(int inNumWeights/*, float *inWeights*/) {
    //allocate the memory
    weights = new float[numWeights];
   
   /*
    //copy the data over into the array
    for(int i = 0; i < numWeights; i++) {
        cout << "\t\t\t inWeights:" << inWeights[i] << endl;
        weights[i] = inWeights[i];
    }
    //set the value as null
    //this will be calculated later
    value = 0;
    */
    
    //used for data check later
    numWeights = inNumWeights;
    currentWeights = 0;
}

node::~node() {
    //deallocate the array
    delete [] weights;
}

void node::addWeight(float val) {
    if(currentWeights < numWeights) {
        weights[currentWeights] = val;
        currentWeights++;
    }
}

void node::setValue(float val) {
    value = val;
}

float node::getValue() {
    //simply return the weights
    return value;
}

float node::getWeight(const int i) {
    //if there are no weights, error out
    if(weights == 0) exit(-42);
    //if the index is outside the array, error out
    else if(i > (numWeights-1)) exit(-43);
    //otherwise, return the value
    else {
        return weights[i];

    }
}