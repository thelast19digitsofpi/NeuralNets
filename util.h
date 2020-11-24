// util.h
//
// things like the sigmoid function, etc

#ifndef UTIL_H_EXISTS
#define UTIL_H_EXISTS

float sigmoidFloat(float);
float sigDerivFloat(float);
// given sigmoid(x), return sigmoid'(x).
float sigOutToDerivFloat(float);
float squareDistance(float*, float*, int);

double sigmoid(double);
double sigDeriv(double);
double sigOutToDeriv(double);
//double squareDistance(double*, double*, int);

#endif

