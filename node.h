class node {
    public:
        //constructors and destructors
        //for the input Nodes
        node();
        //for the output nodes
        node(int inNumWeights/*, double *inWeights*/);
        ~node();
        
        //returns values
        //no direct access to member variables;
        void addWeight(double val);
        void setValue(double val);
        double getValue();
        double getWeight(const int i);
        
    private:
        
        double value;
        double *weights;
        int numWeights;
        int currentWeights;
};