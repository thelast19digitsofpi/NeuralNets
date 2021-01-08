// SigmoidLayer.h
//
// Standard-issue output layer.

#include "SigmoidLayer.h"
#include "util.h"

SigmoidLayer::SigmoidLayer(int givenNodeCount, InputLayer* givenPrev)
: OutputLayer(givenNodeCount, givenPrev) {
	
};

double* SigmoidLayer::advance(double* input, int inputSize) {
	double* output = getWeightedSums(input, inputSize);
	// take the weighted sums and just pass them into sigmoid
	for (int i = 0; i < nodeCount; i++) {
		output[i] = sigmoid(output[i]);
	}
	return output;
};

