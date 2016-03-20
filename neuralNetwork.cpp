#include "neuralNetwork.h"
#include <cstdlib>
#include <fstream>
#include <iostream>

using namespace std;

neuralNetwork::neuralNetwork() {
    //Do stuff here!
}

neuralNetwork::~neuralNetwork() {
    //Delete stuff here!
    
    //delete all the inNodes
    delete [] inNodes;
    //delete all the outNodes
    delete [] outNodes;
    
    //delete the array of arrays
    for(int i = 0; i < numPatterns; i++)
        delete [] patterns[i];
    
    //delete the array of arrays
    for(int i = 0; i < numOutNodes; i++)
        delete [] weights[i];
}

void neuralNetwork::run() {
    //read the data
    if(!readWeights("weights.in")) exit(-1);
    if(!readInputs("patterns.in")) exit(-2);
    
    //double check the files are compatible
    if(numInNodes != numVals) {
        cout << "The number of values per pattern do \
                 not match the number of input nodes.\n\
                 As a result, we must abort." << endl;
        exit(-4);
    }
    
    //allocate the appropriate space
    if(!createNodes()) exit(-3);

    for(int i = 0; i < numPatterns; i++) {
        //update the input values with the
        // next in the pattern!
        updateNodes(patterns[i]);
        
        //calculate the patterns with the
        //i-th set of pattern data
        calculateNodes();
        
        //UPDATE OUTPUT FILE
    }
    
    if(!writeResults()) exit(-99);
    
    cout << "All finished." << endl;
    cout << "Check output.out for the output." << endl;
    
}

bool neuralNetwork::readWeights(string fname) {
    //open the file
    ifstream file;
    
    //open the file
    //If there is a problem,
    //return false
    file.open( fname.c_str() );
        if(file.fail()) return false;
    
    //read the first two numbers
    //and store them in variables
    file >> numInNodes;
    file >> numOutNodes;
    
    //allocate memory or return false
    weights = new float* [numOutNodes];
        if(!weights) return false;
    
    //loop through the number of rows
    for(int i = 0; i < numOutNodes; i++) {
        
        //allocate even more memory
        weights[i] = new float[numInNodes];
            if(!weights[i]) return false;
        
        //loop through the number of columns
        //and stash the weight data
        for(int j = 0; j < numInNodes; j++) {
            file >> weights[i][j];
        }
    }
    
    //make sure to close the file before exiting!
    file.close();
    
    //once everything is finished, return true
    return true;
}

bool neuralNetwork::readInputs(string fname) {
    //open the file
    ifstream file;
    
    //open the file
    //If there is a problem,
    //return false
    file.open( fname.c_str() );
        if(file.fail()) return false;
    
    //read the first three numbers
    //and store them in variables
    file >> numPatterns;
    file >> numVals;
    file >> maxVal;
    
    //allocate memory or return false
    patterns = new float* [numPatterns];
        if(!patterns) return false;
    
    //loop through the number of rows
    for(int i = 0; i < numPatterns; i++) {
        
        //allocate even more memory
        patterns[i] = new float[numVals];
            if(!patterns[i]) return false;
        
        //loop through the number of columns
        //and stash the weight data
        for(int j = 0; j < numVals; j++) {
            file >> patterns[i][j];
        }
    }
    
    //make sure to close the file before exiting!
    file.close();
    
    //once everything is finished, return true
    return true;
}

bool neuralNetwork::writeResults() {
    cout << "Currently underdevelopment!" << endl;
    return true;
}

bool neuralNetwork::createNodes() {
    //allocate the space
    //and create the appropriate nodes
    inNodes = new node*[numInNodes];
        if(!inNodes) return false;
        
    for(int i = 0; i < numInNodes; i++) {
        inNodes[i] = new node();
    }
    
    //allocate the space
    //and create the appropriate nodes
    //and set the weight values to an array
    outNodes = new node*[numOutNodes];
        if(!outNodes) return false;
        
    for(int i = 0; i < numOutNodes; i++) {
        //pass in the respective set of
        //weights to the correct nodes
        outNodes[i] = new node(numInNodes, weights[i]);
    }
    
    //if everything is successful
    //return true
    return true;
}

void neuralNetwork::updateNodes(float *patternSet) {
    for(int i = 0; i < numVals; i++) {
        //update the Node to the new value
        inNodes[i]->setValue(patternSet[i]);
    }
}

void neuralNetwork::calculateNodes() {
    //the number of values in a pattern
    //set is = numVals! Remember that!
    
    //value of output node is...
    //out.value = out[i].weights[j] * in[j].value
    // where out is the output node
    // and in is the input node
    float sum;
    
    for(int i = 0; i < numOutNodes; i++) {
        //clear the junk data
        sum = 0;
        
        //for the number of values
        for(int j = 0; j < numVals; j++) {
            //increase the set sum
            sum += outNodes[i]->getWeight(j) * inNodes[j]->getValue();
        }
        
        //set the sum to the output node value
        outNodes[i]->setValue(sum);
    }
}