// NeuralError.h
//
// wanted a simple error class for all neural network errors

#ifndef NEURALERROR_H_EXISTS
#define NEURALERROR_H_EXISTS

#include <string>

class NeuralError {
public:
	std::string message;
	NeuralError();
	NeuralError(const char*);
	NeuralError(std::string);	
};

#endif
