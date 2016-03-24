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
        delete [] outNodes[i];
    
    //delete the array of arrays
    for(int i = 0; i < numPatterns; i++)
        delete [] patterns[i];
    
    //delete the array of arrays
    for(int i = 0; i < (numHiddenNodes + numOutNodes); i++)
        delete [] weights[i];
}


void neuralNetwork::run() {
    //read the data
    //cout << "Reading the weights..." << endl;
    if(!readWeights("weights.in")) exit(-1);
    //cout << "Reading the patterns..." << endl;
    if(!readInputs("patterns.in")) exit(-2);
    
    /*
    cout << "Write out all the information so far!" << endl;
    cout << "\tIn nodes: " << numInNodes << endl;
    cout << "\tHidden Nodes: " << numHiddenNodes << endl;
    cout << "\tOut Nodes: " << numOutNodes << endl;
    
    cout << "\tWeights:" << endl;
    cout << "\t\tHidden Nodes:" << endl;
    for(int i = 0; i < numHiddenNodes; i++) {
        cout << "\t\t\tHidden node #: " << i << endl;
        for(int j = 0; j < numInNodes; j++) {
            cout << "\t\t\t\tIn Node # " << j << " -- Weight: " << weights[i][j] << endl;
        }
    }
    
    cout << "\t\tOutput Nodes:" << endl;
    for(int i = numHiddenNodes; i < (numHiddenNodes + numOutNodes); i++) {
        cout << "\t\t\tOutput node #: " << i << endl;
        for(int j = 0; j < numHiddenNodes; j++) {
            cout << "\t\t\t\tIn Node # " << j << " -- Weight: " << weights[i][j] << endl;
        }
    }
    
    cout << "End of tests!" << endl;
    
    exit(-999);
    */
    
    //double check the files are compatible
    if(numInNodes != numVals) {
        cout << "The number of values per pattern do \
                 not match the number of input nodes.\n\
                 As a result, we must abort." << endl;
        exit(-4);
    }
    
    //cout << "Creating the nodes next..." << endl;
    //allocate the appropriate space
    if(!createNodes()) exit(-3);
    
    //cout << "Writing the header..." << endl;
    if(!writeHeader()) exit(-6);

    for(int i = 0; i < numPatterns; i++) {
        cout << "Pattern # " << i << endl;
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
    
    //cout << "All finished." << endl;
    //cout << "Check output.out for the output." << endl;
    
}


bool neuralNetwork::readWeights(string fname) {
    //open the file
    ifstream file;
    
    //open the file
    //If there is a problem,
    //return false
    file.open( fname.c_str() );
        if(file.fail()) return false;
    
    //read the first three numbers
    //and store them in variables
    file >> numInNodes;
    file >> numHiddenNodes;
    file >> numOutNodes;
    
    //allocate memory or return false
    //There should be hidden# + output# nodes
    //of rows in the array
    weights = new float* [numHiddenNodes + numOutNodes];
        if(!weights) return false;
        
    // Generate the weights storage for the hidden nodes from input
    for(int i = 0; i < numHiddenNodes; i++) {
        weights[i] = new float[numInNodes];
            if(!weights[i]) return false;
            
        //now we loop through the rest of the data
        for(int j = 0; j < numInNodes; j++) {
            file >> weights[i][j];
        }
    }
    
    //loop through the number of outNodes
    //we can start directly at the number since it will be the next
    //in the set of array indices, assuming I did the math correctly.
    // Also, the number of iterations must be hidden# + out#. Or these
    // weights will never be stored
    for(int i = numHiddenNodes; i < (numOutNodes+numHiddenNodes); i++) {
        
        //allocate even more memory
        weights[i] = new float[numHiddenNodes];
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
    
    //allocate space for the hidden node layer
    hiddenNodes = new node* [numHiddenNodes];
        if(!hiddenNodes) return false;
        
    //create each one individually
    for(int i = 0; i < numHiddenNodes; i++) {
        hiddenNodes[i] = new node(numInNodes);
        
        //loop through and add the weights to
        //the node's array
        for(int j = 0; j < numInNodes; j++){
            hiddenNodes[i]->addWeight(weights[i][j]);
        }
    }
    
    
    //allocate the space
    //and create the appropriate nodes
    //and set the weight values to an array
    outNodes = new node*[numOutNodes];
        if(!outNodes) return false;
        
        
    for(int i = numHiddenNodes; i < (numHiddenNodes + numOutNodes); i++) {
        //pass in the respective set of
        //weights to the correct nodes
        outNodes[i] = new node(numHiddenNodes);
    
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
        //cout << "In the pattern set" << patternSet[i] << endl;
        //cout << "Here it is divided by maxval" << patternSet[i]/maxVal << endl;
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
    
    
    //first let's calculate the hidden nodes
    for(int i = 0; i < numHiddenNodes; i++) {
        //clear the junk data
        sum = 0;
        
        cout << "hidden node#: " << i << endl; 
        
        //for the number of values
        for(int j = 0; j < numInNodes; j++) {
            
	        cout << "\tinnode = " << j << endl;
	        
            //create two temp variables
            //useful for debugging
            float a = hiddenNodes[i]->getWeight(j);
            float b = inNodes[j]->getValue();
       
    	    cout << "\t\tweight = " << a << endl;
    	    cout << "\t\tinput = " << b << endl;     

            //increase the set sum
            sum += a * b;
        }
        
        //sigmoid function
        //sum = sum/sqrt(1+pow(sum,2));
        sum = 1/(1+pow(M_E,(-1*sum)));
        
        //set the sum to the output node value
        hiddenNodes[i]->setValue(sum);
    }
    
    for(int i = numHiddenNodes; i < (numHiddenNodes + numOutNodes); i++) {
        sum = 0;
        
        cout << "Output node# " << i << endl;
        
        for(int j = 0; j < numHiddenNodes; j++) {
            
            cout << "\tHidden Node# " << j << endl;
        
            //create two temp variables
            //useful for debugging
            float a = outNodes[i]->getWeight(j);
            float b = hiddenNodes[j]->getValue();
       
    	    cout << "\t\tweight = " << a << endl;
    	    cout << "\t\tinput = " << b << endl;     

            //increase the set sum
            sum += a * b;
            
        }
        
        //sigmoid function
        //sum = sum/sqrt(1+pow(sum,2));
        sum = 1/(1+pow(M_E,(-1*sum)));
        
        //set the sum to the output node value
        outNodes[i]->setValue(sum);
        
    }
}
