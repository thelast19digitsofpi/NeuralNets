// SigmoidLayer.h
//
// Standard-issue output layer.

#ifndef SIGMOIDLAYER_H_EXISTS
#define SIGMOIDLAYER_H_EXISTS

#include "OutputLayer.h"

class SigmoidLayer: public OutputLayer {
public:
	SigmoidLayer(int, InputLayer*);
	~SigmoidLayer();
	
	double* advance(double*, int);
};

#endif
