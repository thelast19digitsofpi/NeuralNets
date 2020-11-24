// util.cpp
//
// nothing to see here
// just include the header and you're fine

#include "util.h"
#include <cmath>

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

// same stuff but for floats instead of doubles
float sigmoidFloat(float x) {
	return 1.0f / (1.0f + expf(-x));
};

float sigDerivFloat(float x) {
	float expNegX = expf(-x);
	return expNegX / (1.0f + expNegX*(2.0f + expNegX));
};

float sigOutToDerivFloat(float y) {
	return y * (1.0f - y);
};

// The order of "answer" and "expected" is irrelevant
float squareDistance(float* answer, float* expected, int count) {
	float dist = 0;
	for (int i = 0; i < count; i++) {
		float offset = answer[i] - expected[i];
		dist += offset * offset;
	}
	return dist;
};

