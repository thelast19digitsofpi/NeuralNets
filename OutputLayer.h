// OutputLayer.h
//
// This one is actually more interesting.

#ifndef OUTPUTLAYER_H_EXISTS
#define OUTPUTLAYER_H_EXISTS

#include "InputLayer.h"

class OutputLayer: public InputLayer {
protected:
	float** weights;
	float* biases;
public:
	OutputLayer(int, InputLayer*);
	~OutputLayer();
	
	// for direct tuning
	void setWeight(float, int, int);
	void setBias(float, int);
	
	// the easy part: given the previous layer, find this layer's activation
	float* advance(float*, int);
	
	// Learning algorithms
	// All of them take an array of differences
	// Ex: if network returns {0.1, 0.8, 0.3} when {0, 0, 1} was expected,
	// pass {-0.1, -0.2, 0.7} to the function
	float** getWeightChanges(float*, float*);
	// freeing the memory is nontrivial so we provide a method if you know the size
	static void deleteWeightChanges(float**, int);
	// there could be a getBiasChanges but that is just the difference vector itself (or maybe its negative)
	
	// Given a cost list of what changes are requested to this layer
	// we compute the list of changes needed to the previous one
	float* receiveBackPropagate(float*, float*);
};

#endif
