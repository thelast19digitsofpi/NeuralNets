// Network.cpp
//
// Wrapper around that whole structure

#include "Network.h"
#include "util.h"
#include <cstdlib>
#include <iostream>

Network::Network(InputLayer* givenHead) {
	head = givenHead;
	// recursively find the tail
	tail = static_cast<OutputLayer*>(givenHead);
	while (tail->getNext() != nullptr) {
		tail = tail->getNext();
	}
};

Network::~Network() {
	delete head;
};

int Network::getInputSize() {
	return head->getNodeCount();
};

int Network::getNumLayers() {
	// shouldn't ever happen
	if (head == nullptr) {
		return 0;
	}
	// count all layers including input
	int layers = 1;
	OutputLayer* cur = head->getNext();
	while (cur != nullptr) {
		layers++;
		cur = cur->getNext();
	}
	return layers;
}

// The network responds to input!
// note: we usually need to return the entire collection of inputs
double** Network::feedForward(double* input) {
	// Start with the layer just after the start
	OutputLayer* curLayer = head->getNext();
	if (curLayer == nullptr) {
		return nullptr; // probably easier than segfaulting
	}
	
	// this array of arrays is actually a list of each layer's activations
	// the last one corresponds to the output
	// I know getNumLayers() being called here and also outside is wasteful, but meh
	int numLayers = getNumLayers();
	double** outputs = new double*[numLayers];
	
	// copy input into outputs
	// we do this so input and outputs can be safely and separately deleted
	int numNodes = head->getNodeCount();
	outputs[0] = new double[numNodes];
	for (int i = 0; i < numNodes; i++) {
		outputs[0][i] = input[i];
	}
	
	// now the real work
	for (int i = 1; i < numLayers; i++) {
		// assume cur is non-null here (if not and we segfault, that's good to know!)
		if (numNodes != curLayer->getPrev()->getNodeCount()) {
			exit(99); // panic
		}
		outputs[i] = curLayer->advance(outputs[i-1], numNodes);
		
		// move "cur" along
		// normally the "i" loop will end things
		// but also set numNodes to how many nodes the last curLayer had
		numNodes = curLayer->getNodeCount();
		curLayer = curLayer->getNext();
	}
	
	return outputs;
};

// memory freeing
void Network::freeFeedForward(double** data) {
	// which is also the number of rows in data
	int numLayers = getNumLayers();
	for (int i = 0; i < numLayers; i++) {
		delete[] data[i];
	}
	delete[] data;
};

// Updates the entire network!
void Network::update(double** activationList, double* expectedOutput, double scaleFactor) {
	// where are we?
	int numLayers = this->getNumLayers();
	
	// activationList[i] is the activation of layer i (input is layer 0)
	// we want to start at the final output...
	double* layerError = tail->getOutputError(expectedOutput, activationList[numLayers - 1]);
	OutputLayer* curLayer = tail;
	OutputLayer* prevLayer = nullptr;
	
	// back-propagate!
	for (int i = numLayers - 1; i >= 1; i--) {
		// stop at 1 because the input layer is never adjusted
		// activationList[i] is the list of errors for layer i (0 = input layer)
		double** weightChanges = curLayer->getWeightChanges(layerError, activationList[i - 1]);
		
		// we have to compute the next layer's error BEFORE applying the weight changes
		double* prevError = nullptr;
		// if there is another layer behind this one...
		if (i > 1) {
			prevLayer = static_cast<OutputLayer*>(curLayer->getPrev());
			prevError = prevLayer->receiveBackPropagate(layerError, activationList[i - 1]);
		}
		
		// biasChanges is just layerError
		curLayer->applyWeightChanges(weightChanges, scaleFactor);
		curLayer->applyBiasChanges(layerError, scaleFactor);
		
		// free the current error list
		delete[] layerError;
		freeMetaArray(weightChanges, curLayer->getNodeCount());
		// c++ casting stuff
		if (i > 1) {
			// move curLayer back, but only as long as it still points to an OutputLayer
			curLayer = prevLayer;
			// put the next error into layerError
			layerError = prevError;
		}
	}
	
	// anything else...?
	// nope.
};

void Network::printOutput(double** activationList, bool singleLine) {
	InputLayer* curLayer = head;
	int i = 0;
	while (curLayer != nullptr) {
		int numNodes = curLayer->getNodeCount();
		std::cout << "[";
		for (int j = 0; j < numNodes; j++) {
			if (activationList[i][j] < 0.333f) {
				std::cout << RED;
			} else if (activationList[i][j] < 0.667f) {
				std::cout << YELLOW;
			} else {
				std::cout << GREEN;
			}
			std::cout << activationList[i][j] << BLACK;
			if (j < numNodes - 1) {
				std::cout << ", ";
			}
		}
		std::cout << "] ";
		if (!singleLine) {
			std::cout << std::endl;
		}
		
		// c++, you're killing me
		i++;
		curLayer = static_cast<InputLayer*>(curLayer->getNext());
	}	
};
