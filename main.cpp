
#include <iostream>
#include <iomanip>

#include <cstdlib>
#include <ctime>

#include "util.h"
#include "Network.h"
#include "SigmoidLayer.h"
#include "InputLayer.h"

void testChoiceNetwork();
void testUpdateNetwork();
void testAddNetwork();
void testMultNetwork();



int main() {
	srand(time(NULL));
	testAddNetwork();
}



void testAddNetwork() {
	// a deeper neural network with 
	InputLayer* inputs = new InputLayer(8);
	SigmoidLayer* hidden1 = new SigmoidLayer(10, inputs);
	SigmoidLayer* hidden2 = new SigmoidLayer(7, hidden1);
	SigmoidLayer* outputs = new SigmoidLayer(5, hidden2);
	Network myNet(inputs);
	
	hidden1->randomize();
	hidden2->randomize();
	outputs->randomize();
	
	std::cout.precision(3);
	std::cout.setf(std::ios::fixed);
	std::cout << BLUE << "=== TRAINING NETWORK ===" << BLACK << std::endl;
	
	double basicLearningRate = 0.5f;
	
	double in[8];
	double expected[5];
	
	// this one will work a little differently
	
	// training
	int maxEpoch = 750;
	for (int epoch = 0; epoch < maxEpoch; epoch++) {
		double learningRate = basicLearningRate / (1.0f + epoch/7.5f);
		if (1.0f + epoch/7.5f == 0) {
			exit(101);
		}
		
		for (int i = 0; i < 2200; i++) {
			int firstNumber = rand() % 16;
			int secondNumber = rand() % 16;
			// this is the addition network
			int target = firstNumber + secondNumber;
			
			fillBinary(firstNumber, in, 4);
			// put the second number 4 spots later
			fillBinary(secondNumber, in + 4, 4);
			fillBinary(target, expected, 5);
			
			double** data = myNet.feedForward(in);
			myNet.update(data, expected, learningRate);
			
			freeMetaArray(data, 4);
		}
		
		// test
		int totalCost = 0;
		int max = 256;
		for (int i = 0; i < max; i++) {
			int firstNumber = i & 0xf;
			int secondNumber = i >> 4;
			int target = firstNumber + secondNumber;
			
			// technically this reverses the position of the numbers
			// but addition is commutative
			fillBinary(i, in, 8);
			fillBinary(target, expected, 5);
			
			double** data = myNet.feedForward(in);
			totalCost += checkBinary(target, data[3], 5);
			if (epoch == maxEpoch - 1 && randDouble(0.0, 1.0) < 0.07) {
				myNet.printOutput(data, false);
				for (int j = 0; j < 5; j++) {
					std::cout << expected[j] << ' ';
				}
				std::cout << std::endl;
			}
			freeMetaArray(data, 4);
		}
		if (epoch == maxEpoch - 1) {
			std::cout << "Score: ";
			// not an activation but uses the same scale
			printActivation(1.0 - (double)totalCost / (max * 5.0));
			std::cout << std::endl;
		}
		
		if (epoch % 25 == 24) {
			std::cout << "Bit errors: " << totalCost << "/1024" << std::endl;
		}
	}
};


// also the ternary operator but this time learning as it goes
void testUpdateNetwork() {
	InputLayer* il = new InputLayer(3);
	SigmoidLayer* ml = new SigmoidLayer(2, il);
	SigmoidLayer* ol = new SigmoidLayer(1, ml);
	Network myNet(il);
	
	// scramble the weights
	ml->randomize();
	ol->randomize();
	
	std::cout.precision(4);
	std::cout.setf(std::ios::fixed);
	std::cout << BLUE << "=== TRAINING NETWORK ===" << BLACK << std::endl;

	double learningRate = 0.25f; // kind of high so we can see it
	
	double in[3];
	double expected[1];
	
	int max = 8000;
	for (int i = 0; i < max; i++) {
		// random training example
		fillBinary(i, in, 3);
		
		// the "choice" function
		expected[0] = (in[0] > 0.5f ? in[1] : in[2]);
		
		double** data = myNet.feedForward(in);
		// see what it is doing
		if (max - i <= 8) {
			myNet.printOutput(data, true);
			std::cout
				<< "wants: " << expected[0] << std::endl
				<< "diff: " << (data[2][0] - expected[0]) << std::endl;
			std::cout << std::endl;
			
			// print the weight and bias matrix
			// std::cout << "input to middle" << std::endl;
			// ml->printWeightsAndBiases();
			// std::cout << "middle to output" << std::endl;
			// ol->printWeightsAndBiases();
			
			// std::cout << std::endl << std::endl;
		}
		
		myNet.update(data, expected, learningRate);
		
		freeMetaArray(data, 3);
	}
	
	
};


// very very simple network
// function trained on the ternary operator (i.e. a ? b : c) with all three booleans
// in theory a more sophisticated network could do linear interpolation?
void testChoiceNetwork() {
	InputLayer* il = new InputLayer(3);
	SigmoidLayer* ml = new SigmoidLayer(2, il);
	SigmoidLayer* ol = new SigmoidLayer(1, ml);
	Network myNet(il);
	// reasonably well-trained choice gate or ternary operator
	// f(A,B,C) = A ? B : C
	// (A and B) or (not A and C)
	ml->setWeight(20.0f, 0, 0);
	ml->setWeight(-20.0f, 1, 0);
	ml->setWeight(20.0f, 0, 1);
	ml->setWeight(20.0f, 1, 2);
	ml->setBias(-30.0f, 0);
	ml->setBias(-10.0f, 1);
	
	ol->setWeight(20.0f, 0, 0);
	ol->setWeight(20.0f, 0, 1);
	ol->setBias(-10.0f, 0);
	
	// the old way
	std::cout.precision(3);
	std::cout.setf(std::ios::fixed);
	std::cout << BLUE << "Old way: " << BLACK << std::endl;
	for (int i = 0; i <= 7; i++) {
		double in1[3] = {(double)(i/4), (double)((i/2)%2), (double)(i%2)};
		double* pass1 = ml->advance(in1, il->getNodeCount());
		double* pass2 = ol->advance(pass1, ml->getNodeCount());
		
		
		std::cout << "Input: [" << in1[0] << ", " << in1[1] << ", " << in1[2] << "] "
			<< "Middle: [" << pass1[0] << ", " << pass1[1] << "] "
			<< "Output: [" << pass2[0] << "] "
		<< std::endl;
		delete[] pass1;
		delete[] pass2;
	}
	
	std::cout << RED << "New way: " << BLACK << std::endl;
	for (int i = 0; i <= 7; i++) {
		double in1[3] = {(double)(i/4), (double)((i/2)%2), (double)(i%2)};
		double** data = myNet.feedForward(in1);
		
		// printing this is a challenge
		myNet.printOutput(data, true);
		
		freeMetaArray(data, 3);
		std::cout << std::endl;
	}
};

