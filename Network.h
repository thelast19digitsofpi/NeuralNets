// Network.h
//
// Yeah, I do need a class for a basic network.
// This handles evaluating a network and back-propagation.

#ifndef NETWORK_H_EXISTS
#define NETWORK_H_EXISTS

#include "InputLayer.h"
#include "OutputLayer.h"

// Constructing a Network implies the existence of the set of Layers
// The Network just holds it all together

class Network {
protected:
	InputLayer* head;
	OutputLayer* tail;
public:
	Network(InputLayer);
	~Network();
	
	// the network complains if you feed an input of the wrong dimensions
	// it's polite to have a way to get the correct dimensions then
	int getInputSize();
	
	// Feed forward.
	float* evaluate(float*);
	
	// Back propagation!
	float* adjustBiases(float**, int exampleCount);
};


#endif
