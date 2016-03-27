#include <string>
#include "node.h"

using namespace std;

class neuralNetwork {
    public:
        neuralNetwork();
        ~neuralNetwork();
        
        //This function runs the functions
        //and eventually prints the final
        //data in the out file.
        // Also calls the appropciate
        // garbage cleanup at the end.
        void run();
        void run(int debug = 0);
        
    private:
        /*******************
         * Functions *******
         ******************/
        // Parse the input file and store the variables
        bool readWeights(string fname);
        // Parse the pattern input and store the variables
        // Also allocates the space for the answers 2D array
        bool readInputs(string fname);
        // Parse the goal values so as to calculate error later
        bool readCorrect(string fname);
        // Write the appropriate header in the output file
        bool writeHeader(string fname);
        // Write the results of the summation/sigmoid in the file
        bool writeResults(string fname);
        // Create the appropriate number of nodes to run
        bool createNodes();
        //updates the Nodes to reflect the new pattern values
        void updateNodes(float *patternSet);
        // updates the information in the answers array to hold the new answers
        void storeAnswers(int index);
        // Run the summation on all the output nodes
        void calculateNodes();
        // Calculate the error of the given vs. the goal
        void calculateError();
        
        /*******************
         * DATA ************
         ******************/
        // The number of input nodes
        int numInNodes;
        // The number of hidden nodes
        int numHiddenNodes;
        // The number of output nodes
        int numOutNodes;
        
        // The number of patterns being tested
        int numPatterns;
        // The number of values per pattern
        int numVals;
        // The maximum value that a value can be in the pattern
        int maxVal;
        
        //number of correct value patterns
        int numCorrectPatterns;
        //number of expected output nodes
        int numCorrectOutNodes;
        
        // Calculated error of the whole system
        float calculatedError
        
        // The dynamically allocated array of nodes
        // that are described in the numbers above
        // num of of nodes = numInNodes + numOutNodes
        // IMPORTANT :: IN NODES ALWAYS FIRST IN ARRAY
        node **inNodes;
        node **hiddenNodes;
        node **outNodes;
        
        //Create a 2 dimensional array to hold
        // the data for the patterns.
        float **patterns;
        
        //create a 2 dimensional array to hold
        // the weights until they can be used
        // to create the nodes.
        float **weights;
        
        //create a 2D array to hold all the
        // correct values to be compared later
        float **correct;
        
        //create a 2D array to hold all the
        // previous answers
        float **answers;
};