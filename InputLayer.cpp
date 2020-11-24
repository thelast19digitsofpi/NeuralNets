// InputLayer.cpp
//
// Receives the input, I guess?
// Still trying to work that piece of it out

#include "InputLayer.h"
#include "OutputLayer.h"

InputLayer::InputLayer(int givenNodeCount) {
	nodeCount = givenNodeCount;
	next = nullptr;
	prev = nullptr;
};

InputLayer::~InputLayer() {
	// deconstruct from the input layer forward
	if (next != nullptr) {
		delete next;
	}
};

// Getters and setters
int InputLayer::getNodeCount() {
	return nodeCount;
};
InputLayer* InputLayer::getPrev() {
	return prev;
};
OutputLayer* InputLayer::getNext() {
	return next;
};
void InputLayer::setNext(OutputLayer* givenNext) {
	next = givenNext;
};
