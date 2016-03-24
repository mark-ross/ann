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
        
    private:
        /*******************
         * Functions *******
         ******************/
        // Parse the input file and store the variables
        bool readWeights(string fname);
        // Parse the pattern input and store the variables
        bool readInputs(string fname);
        // Write the appropriate header in the output file
        bool writeHeader();
        // Write the results of the summation/sigmoid in the file
        bool writeResults();
        // Create the appropriate number of nodes to run
        bool createNodes();
        //updates the Nodes to reflect the new pattern values
        void updateNodes(float *patternSet);
        // Run the summation on all the output nodes
        void calculateNodes();
        
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
};