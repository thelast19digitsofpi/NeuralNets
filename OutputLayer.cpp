// OutputLayer.cpp
//
// This is going to get real interesting here...


#include "OutputLayer.h"
#include "util.h"
#include "NeuralError.h"

#include <iostream>
#include <cmath>

OutputLayer::OutputLayer(int givenNodeCount, InputLayer* givenPrev)
: InputLayer(givenNodeCount) {
	nodeCount = givenNodeCount;
	prev = givenPrev;
	next = nullptr;
	
	// also make sure the previous node gets a pointer to this
	givenPrev->setNext(this);
	
	// initialize weights and biases
	weights = new double*[givenNodeCount];
	// columns for however many nodes they have
	int theirNodeCount = givenPrev->getNodeCount();
	for (int i = 0; i < givenNodeCount; i++) {
		weights[i] = new double[theirNodeCount];
		// clear it with zeros
		for (int j = 0; j < theirNodeCount; j++) {
			weights[i][j] = 0.0f;
		}
	}
	
	// biases is just a vector
	biases = new double[givenNodeCount];
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

void OutputLayer::setWeight(double value, int row, int col) {
	weights[row][col] = value;
};
void OutputLayer::setBias(double value, int row) {
	biases[row] = value;
};

void OutputLayer::randomize() {
	int theirNodeCount = prev->getNodeCount();
	// prevent early numbers from getting out of hand
	if (theirNodeCount == 0) {
		exit(100);
	}
	double weightRange = 1.0f / theirNodeCount;
	for (int i = 0; i < nodeCount; i++) {
		for (int j = 0; j < theirNodeCount; j++) {
			weights[i][j] = randDouble(-weightRange, weightRange);
		}
		biases[i] = randDouble(-1.0f, 1.0f);
	}
};


void OutputLayer::printWeightsAndBiases() {
	int theirNodeCount = prev->getNodeCount();
	for (int i = 0; i < nodeCount; i++) {
		for (int j = 0; j < theirNodeCount; j++) {
			double weight = weights[i][j];
			// color code
			if (weight < -0.75) {
				std::cout << RED;
			} else if (weight > 0.75) {
				std::cout << GREEN;
			} else {
				std::cout << YELLOW;
			}
			if (weight > 0.0f) {
				std::cout << '+';
			} else if (weight == 0.0f) {
				std::cout << ' ';
			}
			std::cout << weight << ' ' << BLACK;
		}
		std::cout << " | " << biases[i] << std::endl;
	}
};


// computes the raw weighted sums
// this gets used by advance()
double* OutputLayer::getWeightedSums(double* input, int inputSize) {
	double* output = new double[nodeCount];
	for (int i = 0; i < nodeCount; i++) {
		// simple matrix multiplication
		// start with the bias
		double sum = biases[i];
		// inputSize will always be the previous layer's nodeCount
		for (int j = 0; j < inputSize; j++) {
			// multiply the matrix element by the corresponding element of input
			sum += (weights[i][j]) * (input[j]);
		}
		
		// dont forget the bias
		output[i] = sum;
		
		if (std::isnan(output[i])) {
			std::cerr << RED << "Error: NaN detected in output " << i << std::endl;
			throw NeuralError("Error: NaN in getWeightedSums");
		}
	}
	
	return output;
};


// Computational.
// Takes in input from previous layer.


// Learning is going to be difficult...
// neuron i in this layer needs to change by difference[i]
double** OutputLayer::getWeightChanges(double* difference, double* previousActivation) {
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
	double** array = new double*[nodeCount];
	int cols = prev->getNodeCount();
	for (int i = 0; i < nodeCount; i++) {
		array[i] = new double[cols];
		// if we already know what neuron i needs to change by, this is easy
		for (int j = 0; j < cols; j++) {
			array[i][j] = difference[i] * previousActivation[j];
			
			if (std::isnan(difference[i])) {
				std::cout << "difference[" << i << "] is nan" << std::endl;
				exit(107);
			}
			if (std::isnan(previousActivation[j])) {
				std::cout << "previousActivation[" << j << "] is nan" << std::endl;
				exit(108);
			}
		}
	}
	return array;
};

// not sure why I split this into two functions
// maybe because of scaleFactor?
void OutputLayer::applyWeightChanges(double** weightChanges, double scaleFactor) {
	if (std::isnan(scaleFactor)) {
		std::cout << "scaleFactor is nan" << std::endl;
		exit(106);
	}
	
	int theirNodeCount = prev->getNodeCount();
	// std::cout << "applyWeightChanges: double[" << nodeCount << "][" << theirNodeCount << "] " << weightChanges << std::endl;
	for (int i = 0; i < nodeCount; i++) {
		for (int j = 0; j < theirNodeCount; j++) {
			// subtract from weights[i][j] so the difference gets smaller
			weights[i][j] -= weightChanges[i][j] * scaleFactor;
			if (std::isnan(weightChanges[i][j])) {
				std::cout << "weightChanges[" << i << "][" << j << "] is nan" << std::endl;
				exit(109);
			}
		}
	}
};

void OutputLayer::deleteWeightChanges(double** weightChanges, int rowCount) {
	for (int i = 0; i < rowCount; i++) {
		delete[] weightChanges[i];
	}
	delete[] weightChanges;
};

// no need for getBiasChanges, I think, since that is just the error list
void OutputLayer::applyBiasChanges(double* difference, double scaleFactor) {
	// same logic as applyWeightChanges
	// because of the half-sum-square cost function the error is precisely the difference
	for (int i = 0; i < nodeCount; i++) {
		biases[i] -= difference[i] * scaleFactor;
		if (std::isnan(difference[i])) {
			std::cout << "difference[" << i << "] is nan" << std::endl;
			exit(110);
		}
	}
};

// note: here the second parameter is our activation, not the previous one's
double* OutputLayer::receiveBackPropagate(double* nextDifference, double* ourActivation) {
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
	double* array = new double[nodeCount];
	// std::cout << "[receiveBackPropagate] making an array of size " << nodeCount << std::endl;
	
	double** nextWeights = next->weights;
	
	int theirNodeCount = next->nodeCount;
	for (int i = 0; i < nodeCount; i++) {
		double sum = 0;
		// multiply the weight from theirs by ours
		for (int j = 0; j < theirNodeCount; j++) {
			sum += nextWeights[j][i] * nextDifference[j];
			
			if (std::isnan(nextWeights[j][i])) {
				std::cout << "nextWeights[" << j << "][" << i << "] is nan" << std::endl;
				exit(114);
			}
			
			if (std::isnan(nextDifference[j])) {
				std::cout << "nextDifference[" << j << "] is nan" << std::endl;
				exit(115);
			}
		}
		// ourActivation[i] is an array of activations, i.e. s(x)
		// we want the derivative s'(x), which is easily computed
		array[i] = sum * sigOutToDeriv(ourActivation[i]);
		// std::cout << "[receiveBackPropagate]    array[" << i << "] = "
		// 	<< sum << " * "
		// 	<< sigOutToDerivFloat(ourActivation[i]) << std::endl;
		
		
		if (std::isnan(ourActivation[i])) {
			std::cout << "ourActivation[" << i << "] is nan" << std::endl;
			exit(113);
		}
		if (std::isnan(sum)) {
			std::cout << "sum is nan" << std::endl;
			exit(118);
		}
	}
	
	return array;
};

// this honestly could even be static, but we would need an int parameter
double* OutputLayer::getOutputError(double* expected, double* actual) {
	// std::cout << "[getOutputError] nodeCount is " << nodeCount << std::endl;
	double* array = new double[nodeCount];
	// error in the sense of http://neuralnetworksanddeeplearning.com/chap2.html
	for (int i = 0; i < nodeCount; i++) {
		// error times derivative of sigmoid
		array[i] = (actual[i] - expected[i]);
		
		if (std::isnan(expected[i])) {
			std::cout << "expected[" << i << "] is nan" << std::endl;
			exit(111);
		}
		if (std::isnan(actual[i])) {
			std::cout << "actual[" << i << "] is nan" << std::endl;
			exit(112);
		}
	}
	return array;
};