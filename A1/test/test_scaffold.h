/*
 * Queue.h
 *
 *  Created on: Feb 23, 2014
 */

#ifndef TESTSCAFFOLD_H_
#define TESTSCAFFOLD_H_

#include <string>
#include <vector>
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

#define ASSERT(x,y) if(!(x)) return y;

class UnitTest {
public:
	typedef int (*TestFunctionPtr)();

	UnitTest(std::string n) : _unit_name(n) {}

	virtual ~UnitTest() {}

	void attach_test(TestFunctionPtr f, const char * n) {
		_tests.push_back(Test(f,n));
	}

	void run() {
		std::vector<Test>::iterator it = _tests.begin();
		std::cout << "Starting tests for unit: " << _unit_name << std::endl;
		for(; it != _tests.end(); ++it) {
			(std::cout << "\t" << it->_name << ": ").flush();
			if(fork() == 0) {
				exit(it->_func());
			}
			else {
				int status = 0;
				wait(&status);
				if(!WIFEXITED(status))
					std::cout << "ABORTED with signal " << WTERMSIG(status) << std::endl;
				else if(WEXITSTATUS(status) == 0)
					std::cout << "OK!" << std::endl;
				else
					std::cout << "FAILED! (returned: " << WEXITSTATUS(status) << ")" << std::endl;
			}
		}
	}

private:
	std::string _unit_name;

	struct Test {
		Test(TestFunctionPtr f, const char * n) : _func(f), _name(n) {}

		TestFunctionPtr _func;
		std::string _name;
	};
	std::vector<Test> _tests;
};

#endif //TESTSCAFFOLD_H_
