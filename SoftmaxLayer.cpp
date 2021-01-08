// SoftmaxLayer.cpp
//
// The trick will be to extend OutputLayer in the correct fashion

#include "SoftmaxLayer.h"

#include <cmath>

SoftmaxLayer::SoftmaxLayer(int givenNodeCount, InputLayer* givenPrev)
: OutputLayer(givenNodeCount, givenPrev) {
	// don't think anything needs to happen here...
};

double* SoftmaxLayer::advance(double* input, int inputSize) {
	double* weightedSums = getWeightedSums(input, inputSize);
	// softmax: e^a / e^(a+b+c+d), etc
	double sum = 0.0;
	for (int i = 0; i < nodeCount; i++) {
		sum += weightedSums[i];
	}
	
	double expSum = exp(sum);
	
	// now we can recycle the weightedSums array, saving a heap allocation
	for (int i = 0; i < nodeCount; i++) {
		weightedSums[i] = exp(weightedSums[i]) / expSum;
	}
	// yay, no delete[]!
	return weightedSums;
};
