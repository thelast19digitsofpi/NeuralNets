// NeuralError.cpp
//
// makes this all work

#include "NeuralError.h"
#include <string>

NeuralError::NeuralError() {
	message = std::string();
};

NeuralError::NeuralError(const char* givenCString) {
	message = std::string(givenCString);
};

NeuralError::NeuralError(std::string givenString) {
	message = givenString;
};

