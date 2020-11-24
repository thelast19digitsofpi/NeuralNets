// OutputLayer.cpp
//
// This is going to get real interesting here...


#include "OutputLayer.h"
#include "util.h"

#include <iostream>

OutputLayer::OutputLayer(int givenNodeCount, InputLayer* givenPrev)
: InputLayer(givenNodeCount) {
	nodeCount = givenNodeCount;
	prev = givenPrev;
	next = nullptr;
	
	// also make sure the previous node gets a pointer to this
	givenPrev->setNext(this);
	
	// initialize weights and biases
	weights = new float*[givenNodeCount];
	// columns for however many nodes they have
	int theirNodeCount = givenPrev->getNodeCount();
	for (int i = 0; i < givenNodeCount; i++) {
		weights[i] = new float[theirNodeCount];
		// clear it with zeros
		for (int j = 0; j < theirNodeCount; j++) {
			weights[i][j] = 0.0f;
		}
	}
	
	// biases is just a vector
	biases = new float[givenNodeCount];
	for (int i = 0; i < givenNodeCount; i++) {
		biases[i] = 0.0f;
	}
};

OutputLayer::~OutputLayer() {
	for (int i = 0; i < nodeCount; i++) {
		delete[] weights[i];
	}
	delete[] weights;
	delete[] biases;
};

void OutputLayer::setWeight(float value, int row, int col) {
	weights[row][col] = value;
};
void OutputLayer::setBias(float value, int row) {
	biases[row] = value;
};

// Computational.
float* OutputLayer::advance(float* input, int inputSize) {
	float* output = new float[nodeCount];
	for (int i = 0; i < nodeCount; i++) {
		// simple matrix multiplication
		float sum = 0.0f;
		// inputSize will always be the previous layer's nodeCount
		for (int j = 0; j < inputSize; j++) {
			// multiply the matrix element by the corresponding element of input
			sum += (weights[i][j]) * (input[j]);
		}
		
		// write the sigmoid function of that to output
		output[i] = sigmoidFloat(sum + biases[i]);
	}
	
	return output;
};


// Learning is going to be difficult...
// neuron i in this layer needs to change by difference[i]
float** OutputLayer::getWeightChanges(float* difference, float* previousActivation) {
	/*
	A--1
	 \/
	 /\
	B--2
	
	suppose neuron 1 needs to increase by 0.5
	well, its activation is s(w_A1 * A + w_B1 * B + b_1)
	the derivative wrt w_A1, for instance, is just s'(w_A1*A + w_B1*B + b_1) * A.
	*/
	
	// allocate the array
	float** array = new float*[nodeCount];
	int cols = prev->getNodeCount();
	for (int i = 0; i < nodeCount; i++) {
		array[i] = new float[cols];
		// if we already know what neuron i needs to change by, this is easy
		for (int j = 0; j < cols; j++) {
			array[i][j] = difference[i] * previousActivation[j];
		}
	}
	
	return array;
};

void OutputLayer::deleteWeightChanges(float** weightChanges, int rowCount) {
	for (int i = 0; i < rowCount; i++) {
		delete[] weightChanges[i];
	}
	delete[] weightChanges;
};

// no need for getBiasChanges, I think, since that is just the error list

// note: here the second parameter is our activation, not the previous one's
float* OutputLayer::receiveBackPropagate(float* nextDifference, float* ourActivation) {
	/*
	A--1
	 \/
	 /\
	B--2
	
	we own A,B
	if neuron 1 needs to change by d1 and 2 by d2,
	then A will change by (w_a1 * d1 + w_a2 * d2) * sigDeriv(previousActivation)
	
	we need to borrow the next layer's weights and biases
	*/
	
	// the final layer does not receive a backpropagation
	if (next == nullptr) {
		return nullptr;
	}
	// remember we are now behind the other layer
	// so our index comes second
	float* array = new float[nodeCount];
	
	float** nextWeights = next->weights;
	
	int theirNodeCount = next->nodeCount;
	for (int i = 0; i < nodeCount; i++) {
		float sum = 0;
		// multiply the weight from theirs by ours
		for (int j = 0; j < theirNodeCount; j++) {
			sum += nextWeights[j][i] * nextDifference[j];
		}
		// ourActivation[i] is an array of activations, i.e. s(x)
		// we want the derivative s'(x), which is easily computed
		array[i] = sum * sigOutToDerivFloat(ourActivation[i]);
	}
	
	return array;
};

// this honestly could even be static, but we would need an int parameter
float* OutputLayer::getOutputError(float* expected, float* actual) {
	float* array = new float(nodeCount);
	// error in the sense of http://neuralnetworksanddeeplearning.com/chap2.html
	for (int i = 0; i < nodeCount; i++) {
		// error times derivative of sigmoid
		array[i] = (actual[i] - expected[i]) * sigOutToDerivFloat(actua[i]);
	}
};