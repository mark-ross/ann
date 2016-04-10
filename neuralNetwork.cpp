#include "neuralNetwork.h"
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <cmath>

using namespace std;

float digit_rounding(float num) {
    return (floor(num*100000))/100000;
}

neuralNetwork::neuralNetwork(int argc, char* argv[]) {
    //Do stuff here!
    
    numInNodes, numHiddenNodes, numOutNodes = 0;
    numPatterns, numVals, maxVal = 0;
    systemError = 0.0;
    debug = false;
    folderName = "";
    
    //if there are arguments passed into the program
    if(argc > 1) {
        //for all the arguments
        for(int i = 0; i < argc; i++) {
            //typecast a string into tmp
            string tmp = string(argv[i]);
            
            /********************
             * FLAG CATCHING ****
             *******************/
            if(tmp == "-f") {
                if(i+1 < argc) {
                    folderName = string(argv[i+1]);
                    i++; //no need to check the file name
                } else {
                    cout << "Error in usage:" << endl;
                    cout << "./ann -f <FILE_NAME>" << endl;
                    cout << "Exiting..." << endl;
                    exit(-1);
                }
            } else if(tmp == "-b") {
                //create the backwards propagation
                debug = true;
            }
        }
    }
}


neuralNetwork::~neuralNetwork() {
    //Delete stuff here!
    for(int i = 0; i < numHiddenNodes; i++) 
        delete [] inWeights[i];
    
    for(int i = 0; i < numOutNodes; i++)
        delete [] hiddenWeights[i];
    
    if(debug) {
        //delete the training set
        for(int i = 0; i < numPatterns; i++) {
            delete [] correct[i];
            delete [] error[i];
        }
    }
    //delete all the answers, both for output and hidden
    for(int i = 0; i < numPatterns; i++) {
        delete [] patterns[i];
        delete [] outAnswers[i];
        delete [] hiddenAnswers[i];
    }
}


void neuralNetwork::run() {
    
    fileRead();
    
    cout << "\nTopology:" << endl;
    cout << "\tInput Nodes: " << numInNodes << endl;
    cout << "\tHidden Nodes: " << numHiddenNodes << endl;
    cout << "\tOutput Nodes: " << numOutNodes << endl;
    cout << "\tNumber of Patterns: " << numPatterns << endl;
    

    if(debug) {
        int generation = 0;
        do {
            calculateSystem();
            calculateError();
            updateHiddenWeights();
            updateInputWeights();
            
            if(generation % 10000 == 0)
                cout << "Generation: " << generation << " -- Error: " << systemError << endl;
            generation++;
            
        } while(systemError > 0.000000001);

        cout << "\n\nCorrect -- Goals\t|\tFinal -- Output" << endl;
        for(int i = 0; i < numPatterns; i++) {
            for(int j = 0; j < numOutNodes; j++) {
                cout << correct[i][j] << " ";
            }
            cout << "\t|\t";
            for(int j = 0; j < numOutNodes; j++) {
                cout << outAnswers[i][j] << " ";
            }
            cout << endl;
        }
    } else {
        //Simply run the data given
        calculateSystem();
    
        cout << "\n\nFinal -- Output" << endl;
        for(int i = 0; i < numPatterns; i++) {
            for(int j = 0; j < numOutNodes; j++) {
                cout << outAnswers[i][j] << " ";
            }
            cout << endl;
        }
    }
    
    if(!writeResults())
        cout << "Error writing results of the system" << endl;

    cout << "\n\nAll finished." << endl;
    
}


void neuralNetwork::fileRead() {
    
    //ask the user which folder to look for the files
    //This allows for multiple test cases
    if(folderName == "") {
        cout << "What folder? (no slash) --> ";
        cin >> folderName;
    }

    string weightsFile = folderName + "/" + "weights.in";
    string patternsFile = folderName + "/" + "patterns.in";
    string correctFile = folderName + "/" + "correct.in";
    
    //this is a class variable, doesn't need to be declared again
    outputFile = folderName + "/" + "output.out";
    
    //read the data
    if(!readWeights(weightsFile)) exit(-1);
    if(!readInputs(patternsFile)) exit(-2);
    if(debug == true)
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
        
    cout << "Input to Hidden Weights" << endl;
    // Generate the weights storage for the hidden nodes from input
    for(int i = 0; i < numHiddenNodes; i++) {
        inWeights[i] = new float[numInNodes];
            if(!inWeights[i]) return false;
            
        //now we loop through the rest of the data
        for(int j = 0; j < numInNodes; j++) {
            file >> inWeights[i][j];
            cout << inWeights[i][j] << " ";
        }
        cout << endl;
    }
    
    //allocate memory for the weights
    hiddenWeights = new float* [numOutNodes];
        if(!hiddenWeights) return false;
    
    cout << "\nHidden to Output Weights" << endl;
    for(int i = 0; i < numOutNodes; i++) {
        
        //allocate even more memory
        hiddenWeights[i] = new float[numHiddenNodes];
            if(!hiddenWeights[i]) return false;
        
        //loop through the number of columns
        //and stash the weight data
        for(int j = 0; j < numHiddenNodes; j++) {
            file >> hiddenWeights[i][j];
            cout << hiddenWeights[i][j] << " ";
        }
        cout << endl;
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
    
    cout << "\nPattern values (Normalized)" << endl;
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
            patterns[i][j] /= maxVal;
            cout << patterns[i][j] << " ";
        }
        cout << endl;
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
    int numCorrectOutNodes, numCorrectPatterns;
    file >> numCorrectPatterns;
    file >> numCorrectOutNodes;
    
    if(numOutNodes != numCorrectOutNodes) {
        cout << "File asserts incompatible number of output nodes" << endl;
        cout << "Training set incompatible with current network..." << endl;
        cout << "Terminating program" << endl;
        return false;
    } else if(numCorrectPatterns != numPatterns) {
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
    
    cout << "\nCorrect Values" << endl;
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
            cout << correct[i][j] << " ";
        }
        cout << endl;
    }
    
    //make sure to close the file before exiting!
    file.close();
    
    //once everything is finished, return true
    return true;
}


bool neuralNetwork::writeResults() {
    //create the file
    ofstream file;
    
    //open the file in append mode
    // or return with false upon failure
    file.open( outputFile.c_str() , ios::trunc);
        if(file.fail()) return false;
        
    //simply write the number of patterns checked
    file << numPatterns << "\n";
        
    //write the data of all the output nodes for all patterns
    for(int k = 0; k < numPatterns; k++) {
        for(int j = 0; j < numOutNodes; j++) {
            file << outAnswers[k][j] << " ";
        }
        file << "\n";
    }

    file.close();
    return true;
}


bool neuralNetwork::writeSystemError() {
    string fname = folderName + "/" + "error.out";
    ofstream file;
    
    file.open( fname.c_str() , ios::trunc);
        if(file.fail()) return false;
        
    file << digit_rounding(systemError) << "\n";
    
    for(int k = 0; k < numPatterns; k++) {
        for(int i = 0; i < numOutNodes; i++) {
            //set the number of digits to 5 for rounding
            file << digit_rounding(error[k][i]) << " ";
        }
        file << "\n";
    }
}


void neuralNetwork::calculateSystem() {
    cout << "\nBegin Calculating System" << endl;
    
    for(int k = 0; k < numPatterns; k++) {
        cout << "\tFor pattern #" << k << endl;
        float sum;
        
        cout << "\t\tCalculate the Hidden Nodes" << endl;
        //first let's calculate the hidden nodes
        for(int i = 0; i < numHiddenNodes; i++) {
            //clear the junk data
            sum = 0;
            
            cout << "\t\t\tHidden node #" << i << endl;
            //for the number of values
            for(int j = 0; j < numInNodes; j++) {
                //create two temp variables
                //useful for debugging
                float a = inWeights[i][j];
                cout << "\t\t\t\tInput Weight: " << a << endl;
                float b = patterns[k][j];
                cout << "\t\t\t\tInput Value: " << b << endl; 
                //increase the set sum
                sum += a*b;
            }
            
            //sigmoid function
            sum = 1/(1+exp(-sum));
            //set the sum to the output node value
            hiddenAnswers[k][i] = sum;
            cout << "\t\t\tAnswer for hidden Node #" << i << " = " << hiddenAnswers[k][i] << endl;
        }
        
        cout << "\t\tCalculate the OutputNodes" << endl;
        for(int i = 0; i < numOutNodes; i++) {
            sum = 0;  //zero out the sum
            
            cout << "\t\t\tOutput Node #" << i << endl;
            for(int j = 0; j < numHiddenNodes; j++) {
                //create two temp variables
                //useful for debugging
                float a = hiddenWeights[i][j];
                cout << "\t\t\t\tHiddenNode Weight: " << a << endl;
                float b = hiddenAnswers[k][j];
                cout << "\t\t\t\tHiddenNode Value: " << b << endl;
                //increase the set sum
                sum += a*b;
            }
            
            //set the sum to the output node value
            outAnswers[k][i] = sum;
            cout << "\t\t\tAnswer for the outputNode #" << i << " = " << outAnswers[k][i] << endl;
        }
    }
    cout << "Finished" << endl;
}


void neuralNetwork::calculateError() {
    float summation = 0;
    // For all the patterns calculated
    for (int k = 0; k < numPatterns; k++) {
        for(int i = 0; i < numOutNodes; i++) {
            //sigma-k-j in the book
            float a = correct[k][i] - outAnswers[k][i];
            error[k][i] = a;
            //for error in the book
            summation += pow(a,2);
        }
    }
    
    systemError = summation * 0.5;
    writeSystemError();
}


void neuralNetwork::updateHiddenWeights() {
    
    float summation = 0;
    float alpha = 0.1;

    for(int j = 0; j < numOutNodes; j++) {
        for(int i = 0; i < numHiddenNodes; i++) {
            
            //for all the patterns
            for(int k = 0; k < numPatterns; k++) {
                //finish the derivative by multiplying by the
                //yield of hidden node at index i
                summation += -error[k][j] * hiddenAnswers[k][i];
            }
        
        //finally, the weight of index j, i is...
        hiddenWeights[j][i] -= alpha * summation;
        summation = 0;
        }
    }
}

void neuralNetwork::updateInputWeights() {
    float summation = 0;
    float beta = 0.1;
    float temp = 0;
    

    for(int i = 0; i < numHiddenNodes; i++) {
        for(int h = 0; h < numInNodes; h++) {
            for(int k = 0; k < numPatterns; k++) {
                temp = 0;  //zero it out
                
                // first part of sigmoid derivative
                float a = hiddenAnswers[k][i] * (1 - hiddenAnswers[k][i]);
                
                //perform the summation over all the original errors
                for(int j = 0; j < numOutNodes; j++) 
                   temp += error[k][j] * hiddenWeights[j][i];
                
                //summation for the final equation
                summation += -temp * a * patterns[k][h];
            }
            
            //adjust the old weight with the results of the summations
            inWeights[i][h] -= (beta * summation);
            
        }
    }
}