// util.h
//
// things like the sigmoid function, etc

#ifndef UTIL_H_EXISTS
#define UTIL_H_EXISTS

// Automatically include the math library
#include <cmath>

double squareDistance(double*, double*, int);

double sigmoid(double);
double sigDeriv(double);
// given sigmoid(x) outputs sigDeriv(x)
double sigOutToDeriv(double);

// memory management
void freeMetaArray(double**, int);

double randDouble(double min, double max);

// fills number in binary into the doubles
// [0] is the least significant bit
// excess bits are just ignored
void fillBinary(int number, double* startPointer, int numBits);

// checks binary
// anything 2/3 to 1 is considered a "1"
// anything 0 to 1/3 is considered a "0"
// anything in the middle is considered always wrong
int checkBinary(int target, double*, int numBits);

// terminal color output!
#define BLACK "\e[30;0m"
#define RED "\e[31;1m"
#define GREEN "\e[32;1m"
#define BLUE "\e[34;1m"
#define YELLOW "\e[33;1m"

// color printing!
// different thresholds...
void printActivation(double);
void printWeightOrBias(double);

#endif

