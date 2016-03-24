#include "neuralNetwork.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <math.h>

using namespace std;

neuralNetwork::neuralNetwork() {
    //Do stuff here!
}


neuralNetwork::~neuralNetwork() {
    //Delete stuff here!
    
    //delete all the inNodes
    for(int i = 0; i < numInNodes; i++)
        delete [] inNodes[i];
    //delete all hidden nodes
    for(int i = 0; i < numHiddenNodes; i++)
        delete [] hiddenNodes[i];
    //delete all the outNodes
    for(int i = 0; i < numOutNodes; i++)
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
    cout << "Reading the weights..." << endl;
    if(!readWeights("weights.in")) exit(-1);
    cout << "Reading the patterns..." << endl;
    if(!readInputs("patterns.in")) exit(-2);
    
    //double check the files are compatible
    if(numInNodes != numVals) {
        cout << "The number of values per pattern do \
                 not match the number of input nodes.\n\
                 As a result, we must abort." << endl;
        exit(-4);
    }
    
    cout << "Creating the nodes next..." << endl;
    //allocate the appropriate space
    if(!createNodes()) exit(-3);
    
    cout << "Writing the header..." << endl;
    if(!writeHeader()) exit(-6);

    for(int i = 0; i < numPatterns; i++) {
        //cout << "\tUpdating for the next pattern" << endl;
        //update the input values with the
        // next in the pattern!
        updateNodes(patterns[i]);
        
        //cout << "\tCalculating the results" << endl;
        //calculate the patterns with the
        //i-th set of pattern data
        calculateNodes();
        
        //cout << "\tWrite the final stuff in the file" << endl;
        if(!writeResults()) exit(-99);
    }
    
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


bool neuralNetwork::writeHeader() {
    //open the file
    ofstream file;
    //return false if it fails
    file.open("output.out" , ios::trunc);
        if(file.fail()) return false;
    //simply write the number of patterns checked
    file << numPatterns << "\n";
    //write the file and close
    file.close();
    //finally return true
    return true;
}

bool neuralNetwork::writeResults() {
    //create the file
    ofstream file;
    //open the file in append mode
    // or return with false upon failure
    file.open("output.out" , ios::app);
        if(file.fail()) return false;
        
    //write the data of all the output nodes
    for(int i = 0; i < numOutNodes; i++) {
        file << outNodes[i]->getValue() << " ";
    }
    //add a new line for clarity
    file << "\n";
    //close the file
    file.close();
    //return
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
        outNodes[i] = new node(numInNodes);
    
        for(int j = 0; j < numInNodes; j++) {
            outNodes[i]->addWeight(weights[i][j]);
        }
        
    }
    
    //if everything is successful
    //return true
    return true;
}


void neuralNetwork::updateNodes(float *patternSet) {
    //We must divide by the maxVal in order
    //to normalize the data
    for(int i = 0; i < numVals; i++) {
        //update the Node to the new value
        inNodes[i]->setValue(patternSet[i]/maxVal);
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
            //create two temp variables
            //useful for debugging
            float a = outNodes[i]->getWeight(j);
            float b = inNodes[j]->getValue();
            
            //increase the set sum
            sum += a * b;
        }
        
        //sigmoid function
        sum = sum/sqrt(1+pow(sum,2));
        
        //set the sum to the output node value
        outNodes[i]->setValue(sum);
    }
}