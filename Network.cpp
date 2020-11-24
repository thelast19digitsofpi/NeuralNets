// Network.cpp
//
// Wrapper around that whole structure

#include "Network.h"


Network::Network(InputLayer* givenHead) {
	head = givenHead;
	// recursively find the tail
	tail = givenHead;
	while (tail->next != nullptr) {
		tail = tail->next;
	}
};

Network::~Network() {
	delete head;
};

