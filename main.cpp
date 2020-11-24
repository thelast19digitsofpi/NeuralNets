
#include <iostream>

#include "OutputLayer.h"
#include "InputLayer.h"

int main() {
	InputLayer* il = new InputLayer(3);
	OutputLayer* ml = new OutputLayer(2, il);
	OutputLayer* ol = new OutputLayer(1, ml);
	
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
	
	for (int i = 0; i <= 7; i++) {
		float in1[3] = {(float)(i/4), (float)((i/2)%2), (float)(i%2)};
		float* pass1 = ml->advance(in1, il->getNodeCount());
		float* pass2 = ol->advance(pass1, ml->getNodeCount());
		
		std::cout << "Input: " << in1[0] << ", " << in1[1] << ", " << in1[2] << std::endl;
		std::cout << "Middle: " << pass1[0] << ", " << pass1[1] << std::endl;
		std::cout << "Output: " << pass2[0] << std::endl;
		std::cout << std::endl;
		delete[] pass1;
		delete[] pass2;
	}
	delete il;
};

