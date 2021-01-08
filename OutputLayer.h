// OutputLayer.h
//
// This one is actually more interesting.

#ifndef OUTPUTLAYER_H_EXISTS
#define OUTPUTLAYER_H_EXISTS

#include "InputLayer.h"

class OutputLayer: public InputLayer {
protected:
	double** weights;
	double* biases;
public:
	OutputLayer(int, InputLayer*);
	~OutputLayer();
	
	// for direct tuning
	void setWeight(double, int, int);
	void setBias(double, int);
	
	void printWeightsAndBiases();
	
	// for scrambling
	void randomize();
	
	// gets the weighted sums
	// most types of layers will use this
	double* getWeightedSums(double*, int);
	
	// the easy part: given the previous layer, find this layer's activation
	virtual double* advance(double*, int) = 0;
	
	// Learning algorithms
	// All of them take an array of differences
	// Ex: if network returns {0.1, 0.8, 0.3} when {0, 0, 1} was expected,
	// pass {-0.1, -0.2, 0.7} to the function
	double** getWeightChanges(double*, double*);
	void applyWeightChanges(double**, double);
	// freeing the memory is nontrivial so we provide a method if you know the size
	static void deleteWeightChanges(double**, int);
	// there could be a getBiasChanges but that is just the difference vector itself (or maybe its negative)
	
	void applyBiasChanges(double*, double);
	
	// Given a cost list of what changes are requested to this layer
	// we compute the list of changes needed to the previous one
	double* receiveBackPropagate(double*, double*);
	
	// If this is truly the tail, we need to get the error based on an expected output
	double* getOutputError(double*, double*);
};

#endif
