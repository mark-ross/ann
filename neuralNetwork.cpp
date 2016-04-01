#include "neuralNetwork.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

neuralNetwork::neuralNetwork() {
    //Do stuff here!
}


neuralNetwork::~neuralNetwork() {
    //Delete stuff here!
    
    //cout << "Deleting In nodes" << endl;
    //delete all the inNodes
    for(int i = 0; i < numInNodes; i++)
        delete inNodes[i];
        
    //cout << "Deleting hidden nodes" << endl;
    //delete all hidden nodes
    for(int i = 0; i < numHiddenNodes; i++)
        delete hiddenNodes[i];
        
    //cout << "Deleting output nodes" << endl;
    //delete all the outNodes
    for(int i = 0; i < numOutNodes; i++)
        delete outNodes[i];
    
    //cout << "Deleting the patterns" << endl;
    //delete the array of arrays
    for(int i = 0; i < numPatterns; i++)
        delete [] patterns[i];
    
    //cout << "Deleting the stored input weights" << endl;
    //delete the array of arrays
    for(int i = 0; i < numHiddenNodes; i++)
        delete [] inWeights[i];
    
    //cout << "Deleteing the stored hidden weights" << endl;
    for(int i = 0; i < numOutNodes; i++)
        delete [] hiddenWeights[i];
    
    if(debug == true) {
        //cout << "Deleting the training data array" << endl;
        //delete the training set
        for(int i = 0; i < numCorrectPatterns; i++)
            delete [] correct[i];
            
        //cout << "Deleting the error" << endl;
        for(int i = 0; i < numCorrectPatterns; i++)
            delete [] error[i];
    }
        
    //cout << "Deleting the answers given from the system" << endl;
    //delete all the answers, both for output and hidden
    for(int i = 0; i < numPatterns; i++) {
        delete [] outAnswers[i];
        delete [] hiddenAnswers[i];
    }
    
    //cout << "Finished deleting everything" << endl;
}


void neuralNetwork::run(int flag) {
    
    //determine whether to run the
    //correction code or not
    if(flag == 1) debug = true;
    else          debug = false;
    
    fileRead();
    runData();
    
    if(debug) {
        cout << "Running the back propagation stuffs..." << endl;
        calculateError();
        updateHiddenWeights();
    }
    
    cout << "All finished." << endl;
    cout << "Check output.out for the output." << endl;
    
}


void neuralNetwork::fileRead() {
    
    //ask the user which folder to look for the files
    //This allows for multiple test cases
    cout << "What folder? (no slash) --> ";
    cin >> folderName;

    string weightsFile = folderName + "/" + "weights.in";
    string patternsFile = folderName + "/" + "patterns.in";
    string correctFile = folderName + "/" + "correct.in";
    
    //this is a class variable, doesn't need to be declared again
    outputFile = folderName + "/" + "output.out";
    
    //read the data
    //cout << "Reading the weights..." << endl;
    if(!readWeights(weightsFile)) exit(-1);
    //cout << "Reading the patterns..." << endl;
    if(!readInputs(patternsFile)) exit(-2);
    if(debug == true)
        //cout << "Reading the training data" << endl;
        if(!readCorrect(correctFile)) exit(-75);
    
    
    //float check the files are compatible
    if(numInNodes != numVals) {
        cout << "The number of values per pattern do\n"
             << "not match the number of input nodes.\n"
             << "As a result, we must abort." << endl;
        exit(-4);
    }
    
}


bool neuralNetwork::readWeights(string fname) {
    //open the file
    ifstream file;
    
    //open the file
    //If there is a problem,
    //return false
    file.open( fname.c_str() );
        if(file.fail()) {
            cout << "Error opening " << fname << endl;
            cout << "Terminating program" << endl;
            return false;
        }
    
    //read the first three numbers
    //and store them in variables
    file >> numInNodes;
    file >> numHiddenNodes;
    file >> numOutNodes;
    
    //allocate memory or return false
    //There should be hidden# + output# nodes
    //of rows in the array
    inWeights = new float* [numHiddenNodes];
        if(!inWeights) return false;
        
    // Generate the weights storage for the hidden nodes from input
    for(int i = 0; i < numHiddenNodes; i++) {
        inWeights[i] = new float[numInNodes];
            if(!inWeights[i]) return false;
            
        //now we loop through the rest of the data
        for(int j = 0; j < numInNodes; j++) {
            file >> inWeights[i][j];
        }
    }
    
    //loop through the number of outNodes
    //we can start directly at the number since it will be the next
    //in the set of array indices, assuming I did the math correctly.
    // Also, the number of iterations must be hidden# + out#. Or these
    // weights will never be stored
    
    hiddenWeights = new float* [numOutNodes];
        if(!hiddenWeights) return false;
    
    for(int i = 0; i < numOutNodes; i++) {
        
        //allocate even more memory
        hiddenWeights[i] = new float[numHiddenNodes];
            if(!hiddenWeights[i]) return false;
        
        //loop through the number of columns
        //and stash the weight data
        for(int j = 0; j < numHiddenNodes; j++) {
            file >> hiddenWeights[i][j];
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
    
    //If there is a problem,
    //return false
    file.open( fname.c_str() );
        if(file.fail()) {
            cout << "Error opening " << fname << endl;
            cout << "Terminating program" << endl;
            return false;
        }
    
    //read the first three numbers
    //and store them in variables
    file >> numPatterns;
    file >> numVals;
    file >> maxVal;
    
    //allocate memory or return false
    patterns = new float* [numPatterns];
        if(!patterns) return false;
        
    outAnswers = new float* [numPatterns];
        if(!outAnswers) return false;
        
    hiddenAnswers = new float* [numPatterns];
        if(!hiddenAnswers) return false;
    
    //loop through the number of rows
    for(int i = 0; i < numPatterns; i++) {
        
        //allocate even more memory
        patterns[i] = new float[numVals];
            if(!patterns[i]) return false;
            
        //no need to assign anything yet
        //just create the space for later
        outAnswers[i] = new float[numOutNodes];
            if(!outAnswers[i]) return false;
            
        hiddenAnswers[i] = new float[numHiddenNodes];
            if(!hiddenAnswers[i]) return false;
        
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

bool neuralNetwork::readCorrect(string fname) {
    //open the file
    ifstream file;
    
    //If there is a problem,
    //return false
    file.open( fname.c_str() );
        if(file.fail()) {
            cout << "Error opening " << fname << endl;
            cout << "Terminating program" << endl;
            return false;
        }
    
    //read the first three numbers
    //and store them in variables
    file >> numCorrectPatterns;
    file >> numCorrectOutNodes;
    
    if(numOutNodes != numCorrectOutNodes) {
        cout << "File asserts incompatible number of output nodes" << endl;
        cout << "Training set incompatible with current network..." << endl;
        cout << "Terminating program" << endl;
        return false;
    } else if(numCorrectPatterns < numPatterns) {
        cout << "Too few numbers of correct data for this set of patterns" << endl;
        cout << "Please create a comprehensive training set..." << endl;
        cout << "Terminating program" << endl;
        return false;
    }
    
    //allocate memory or return false
    correct = new float* [numCorrectPatterns];
        if(!correct) return false;
        
    //allocate memory for the error array
    error = new float*[numPatterns];
        if(!error) return false;
    
    //loop through the number of rows
    for(int i = 0; i < numCorrectPatterns; i++) {
        
        //allocate even more memory
        correct[i] = new float[numCorrectOutNodes];
            if(!correct[i]) return false;
            
        // For each of the output nodes
        error[i] = new float[numOutNodes];
            if(!error[i]) return false;
            
            
        //loop through the number of columns
        //and stash the weight data
        for(int j = 0; j < numCorrectOutNodes; j++) {
            file >> correct[i][j];
        }
    }
    
    //make sure to close the file before exiting!
    file.close();
    
    //once everything is finished, return true
    return true;
}


bool neuralNetwork::writeHeader(string fname) {
    //open the file
    ofstream file;
    //return false if it fails
    file.open( fname.c_str(), ios::trunc);
        if(file.fail()) return false;
    //simply write the number of patterns checked
    file << numPatterns << "\n";
    //write the file and close
    file.close();
    //finally return true
    return true;
}

bool neuralNetwork::writeResults(string fname) {
    //create the file
    ofstream file;
    //open the file in append mode
    // or return with false upon failure
    file.open( fname.c_str() , ios::app);
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


bool neuralNetwork::writeSystemError() {
    string fname = folderName + "/" + "error.out";
    ofstream file;
    
    file.open( fname.c_str() , ios::trunc);
        if(file.fail()) return false;
        
    file << systemError << "\n";
    
    for(int k = 0; k < numPatterns; k++) {
        for(int i = 0; i < numOutNodes; i++) {
            file << error[k][i] << " ";
        }
        file << "\n";
    }
}


void neuralNetwork::runData() {
    //cout << "Creating the nodes next..." << endl;
    //allocate the appropriate space
    if(!createNodes()) exit(-3);
    
    //cout << "Writing the header..." << endl;
    if(!writeHeader(outputFile)) exit(-6);
    
    for(int i = 0; i < numPatterns; i++) {
        //cout << "Pattern # " << i << endl;
        //cout << "\tUpdating for the next pattern" << endl;
        //update the input values with the
        // next in the pattern!
        updateNodes(patterns[i]);
        
        //cout << "\tCalculating the results" << endl;
        //calculate the patterns with the
        //i-th set of pattern data
        calculateNodes();
        
        //store the answers in the answers 2D array
        storeAnswers(i);
        
        //cout << "\tWrite the final stuff in the file" << endl;
        if(!writeResults(outputFile)) exit(-99);
    }
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
            hiddenNodes[i]->addWeight(inWeights[i][j]);
        }
    }
    
    
    //allocate the space
    //and create the appropriate nodes
    //and set the weight values to an array
    outNodes = new node*[numOutNodes];
        if(!outNodes) return false;
        
        
    for(int i = 0; i < numOutNodes; i++) {
        //pass in the respective set of
        //weights to the correct nodes
        outNodes[i] = new node(numHiddenNodes);
    
        for(int j = 0; j < numHiddenNodes; j++) {
            //access to the weights have to be
            //offset by the number of hidden nodes
            outNodes[i]->addWeight(hiddenWeights[i][j]);
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

void neuralNetwork::storeAnswers(int index) {
    //for all the output nodes
    for(int i = 0; i < numOutNodes; i++) {
        //set the answers array to the value of the outNode.value()
        //Now all the weights can be stored and used later.
        outAnswers[index][i] = outNodes[i]->getValue();
    }
    
    //for all the hidden nodes
    for(int i = 0; i < numHiddenNodes; i++) {
        //set the value in the hiddenAnswer to the value
        //of the hidden nodes. Used in back propagation
        hiddenAnswers[index][i] = hiddenNodes[i]->getValue();
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
    
    //cout << "Calculating Input nodes" << endl;
    
    //first let's calculate the hidden nodes
    for(int i = 0; i < numHiddenNodes; i++) {
        //clear the junk data
        sum = 0;
        
        //cout << "hidden node#: " << i << endl; 
        
        //for the number of values
        for(int j = 0; j < numInNodes; j++) {
            
	        //cout << "\tinnode = " << j << endl;
	        
            //create two temp variables
            //useful for debugging
            float a = hiddenNodes[i]->getWeight(j);
            float b = inNodes[j]->getValue();
       
    	    //cout << "\t\tweight = " << a << endl;
    	    //cout << "\t\tinput = " << b << endl;     

            //increase the set sum
            float result = a*b;
            //cout << "\t\t\tresult of input * weight = " << result << endl;;
            sum += result;
            //cout << "\tsum = " << sum << endl;
            
        }
        
        //sigmoid function
        //sum = sum/sqrt(1+pow(sum,2));
        float exponent = pow(M_E,(-1 * sum));
        exponent++; //add 1 to it.
        sum = 1/exponent;
        
        //cout << "\tAbout to assign the new value " << sum << endl;
        
        //set the sum to the output node value
        hiddenNodes[i]->setValue(sum);
    }
    
    //cout << "Now to the output nodes" << endl;
    
    for(int i = 0; i < numOutNodes; i++) {
        sum = 0;
        
        //cout << "Output node# " << i << endl;
        
        for(int j = 0; j < numHiddenNodes; j++) {
            
            //cout << "\tHidden Node# " << j << endl;
        
            //create two temp variables
            //useful for debugging
            float a = outNodes[i]->getWeight(j);
            float b = hiddenNodes[j]->getValue();
       
    	    //cout << "\t\tweight = " << a << endl;
    	    //cout << "\t\tinput = " << b << endl;     

            //increase the set sum
            float result = a*b;
            //cout << "\t\t\tresult of input * weight = " << result << endl;;
            sum += result;
            //cout << "\tsum = " << sum << endl;
            
        }
        
        //sigmoid function
        //sum = sum/sqrt(1+pow(sum,2));
        float exponent = pow(M_E,(-1 * sum));
        exponent++; //add 1 to it.
        sum = 1/exponent;
        
        //cout << "\tAbout to assign the new value " << sum << endl;
        
        //set the sum to the output node value
        outNodes[i]->setValue(sum);
        
    }
}


void neuralNetwork::calculateError() {
    float summation = 0;
    // For all the patterns calculated
    for (int k = 0; k < numCorrectPatterns; k++) {
        for(int i = 0; i < numOutNodes; i++) {
            cout << "\tGoal answer: " << correct [k][i] << "  Actual answer: " << outAnswers[k][i] << endl;
            float a = abs(correct[k][i] - outAnswers[k][i]);
            cout << "\tResult is: " << a << endl;
            float b = pow(a,2);
            summation += a;
            cout << "\t\tSummation = " << summation << endl;
            error[k][i] = a;
        }
    }
    
    
    systemError = summation * 0.5;
    cout << "total system error = " << systemError << endl;
    
    
    writeSystemError();
}


void neuralNetwork::updateHiddenWeights() {
    
    float summation = 0;
    float alpha = 1.0;

    //for each of the outnodes
    for(int j = 0; j < numOutNodes; j++) {
        //and for each of the hidden nodes
        for(int i = 0; i < numHiddenNodes; i++) {
            //for all the patterns
            for(int k = 0; k < numPatterns; k++) {
                //calculate the sigma value (pp. 185, equation 5.24b)
                float sigma = correct[k][j] - outAnswers[k][j];
                //finally finish the derivative by multiplying by the
                //yield of hidden node at index i
                float derivative = sigma * hiddenAnswers[k][i];
                summation += alpha * derivative;
            }
        
        //finally, the weight of index j, i is...
        hiddenWeights[j][i] = hiddenWeights[j][i] + summation;
        summation = 0;
        }
    }
}

void neuralNetwork::updateInputWeights() {
    float summation = 0;
    float beta = 1.0;
    
    for(int k = 0; k < numPatterns; k++) {
        for(int i = 0; i < numHiddenNodes; i++) {
            
            float a = hiddenAnswers[k][i] * (1 - hiddenAnswers[k][i]);
            
            for(int j = 0; j < numOutNodes; j++) {
                //summation += 
            }
        }
    }
    float derivative_of_k_i;
    
}