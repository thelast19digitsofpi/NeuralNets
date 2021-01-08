// SoftmaxLayer.h
//
// Like an output layer but uses softmax instead of sigmoid for its output

#ifndef SOFTMAXLAYER_H_EXISTS
#define SOFTMAXLAYER_H_EXISTS

#include "OutputLayer.h"

class SoftmaxLayer: public OutputLayer {
public:
	SoftmaxLayer(int, InputLayer*);
	~SoftmaxLayer();
	
	
	double* advance(double*, int);
};

#endif
