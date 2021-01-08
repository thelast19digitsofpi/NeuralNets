// util.cpp
//
// nothing to see here
// just include the header and you're fine

#include "util.h"
#include <cmath>
#include <cstdlib>
#include <iostream>


double sigmoid(double x) {
	return 1.0 / (1.0 + exp(-x));
};

double sigDeriv(double x) {
	// f(x) = 1/(1 + e^-x)
	// f'(x) = e^-x / (1 + e^-x)^2 (some negatives cancel)
	double expNegX = exp(-x);
	// (1+t)^2 = 1 + 2t + t^2 = 1 + t(2 + t)
	return expNegX / (1.0 + expNegX*(2.0 + expNegX));
};

// given f(x) return f'(x)
// substitute the formula for s'(x) into the inverse, it simplifies to y - y^2
double sigOutToDeriv(double y) {
	return y * (1.0 - y);
};

// same stuff but for doubles instead of doubles
double sigmoidFloat(double x) {
	return 1.0f / (1.0f + expf(-x));
};

double sigDerivFloat(double x) {
	double expNegX = expf(-x);
	return expNegX / (1.0f + expNegX*(2.0f + expNegX));
};

double sigOutToDerivFloat(double y) {
	return y * (1.0f - y);
};

// The order of "answer" and "expected" is irrelevant
double squareDistance(double* answer, double* expected, int count) {
	double dist = 0;
	for (int i = 0; i < count; i++) {
		double offset = answer[i] - expected[i];
		dist += offset * offset;
	}
	return dist;
};


// free the arrays of pointers
void freeMetaArray(double** metaArray, int numArrays) {
	for (int i = 0; i < numArrays; i++) {
		delete[] metaArray[i];
		metaArray[i] = nullptr;
	}
	delete[] metaArray;
};


double randDouble(double min, double max) {
	// (max - min) * t + min, where 0 <= t <= 1
	return (max - min) * ((double)rand() / (double)RAND_MAX) + min;
};


void fillBinary(int number, double* startPointer, int numBits) {
	for (int i = 0; i < numBits; i++) {
		// put the 1's bit
		startPointer[i] = (double)(number & 1);
		// and then discard that bit with a right-shift
		number >>= 1;
	}
};

// 0 to 1/3 is zero, 2/3 to 1 is one, in the middle is always wrong
int checkBinary(int target, double* startPointer, int numBits) {
	int wrong = 0;
	for (int i = 0; i < numBits; i++) {
		double value = startPointer[i];
		int bit = (target >> i) & 1;
		// if (low and 0) or (high and 1)
		if ( !(value <= 1.0/3.0 && bit == 0) && !(value >= 2.0/3.0 && bit == 1) ) {
			wrong++;
		}
	}
	return wrong;
};


void printActivation(double x) {
	if (x < 0.334) {
		std::cout << RED;
	} else if (x > 0.666) {
		std::cout << GREEN;
	} else {
		std::cout << YELLOW;
	}
	std::cout << x << BLACK;
};
