/*
* BOOOS.h
*
*  Created on: Aug 14, 2014
*/

#include <BOOOS.h>
#include <Task.h>
#include <iostream>

namespace BOOOS {

BOOOS::BOOOS(bool verbose) : _verbose(verbose) {
	if(_verbose) std::cout << "Welcome to BOOOS - Basic Object Oriented Operating System!" << std::endl;

	// Call init routines of other components

	Task::init();
}

BOOOS::~BOOOS() {
	// Call finish routines of other components (if any)


	if(_verbose) std::cout << "BOOOS ended... Bye!" << std::endl;
}

void BOOOS::panic() {
	std::cerr << "BOOOSta! Panic!" << std::endl;
	while(true);
}

} /* namespace BOOOS */
