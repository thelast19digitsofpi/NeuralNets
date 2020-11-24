// InputLayer.h
//
// There is only one input layer.
// The entire network is essentially a double linked list.
// I think I will require that a network be constructed in the "forward" direction.

#ifndef INPUTLAYER_H_EXISTS
#define INPUTLAYER_H_EXISTS

class OutputLayer;

class InputLayer {
protected:
	int nodeCount;
	InputLayer* prev;
	OutputLayer* next;
public:
	InputLayer(int);
	~InputLayer();
	
	int getNodeCount();
	InputLayer* getPrev();
	OutputLayer* getNext();
	void setNext(OutputLayer*);
};


#endif
