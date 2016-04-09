#include <string>

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
         
        void fileRead();
        // Parse the input file and store the variables
            bool readWeights(string fname);
            // Parse the pattern input and store the variables
            // Also allocates the space for the answers 2D array
            bool readInputs(string fname);
            // Parse the goal values so as to calculate error later
            bool readCorrect(string fname);
            
        // Write the results of the summation/sigmoid in the file
        bool writeResults(string fname);
        // Output the calculated error to a file
        bool writeSystemError();
        // Create the appropriate number of nodes to run
        bool createNodes();
        
        void runData();
            // Run the summation on all the values -- no nodes
            void calculateSystem(int patternNumber);
        
        // Calculate the error of the given vs. the goal
        void calculateError();
        
        // Calculate the difference needed in the weights and
        // then store that new information in the array
        void updateHiddenWeights();
        void updateInputWeights();
        
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
        
        //number for total system error
        float systemError;
        
        //set the flag to determine whether or not to run
        // the backpropagation code
        bool debug;
        
        //save the file name for the output later
        string outputFile;
        string folderName;
        
        //Create a 2 dimensional array to hold
        // the data for the patterns.
        float **patterns;
        
        //create a 2 dimensional array to hold
        // the weights until they can be used
        // to create the nodes.
        float **inWeights;
        float **hiddenWeights;
        
        //create a 2D array to hold all the
        // correct values to be compared later
        float **correct;
        
        //create a 2D array to hold all the
        // previous answers
        float **outAnswers;
        
        //create a 2D array to hold the hidden
        // layer results
        float **hiddenAnswers;
        
        //create a 2D array to hold all the
        // calculated errors of the system
        float **error;
};