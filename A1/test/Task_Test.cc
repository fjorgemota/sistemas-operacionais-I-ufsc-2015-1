/*
 * Queue_Test.cc
 *
 *  Created on: Feb 21, 2014
 */

#include <iostream>
#include <queue>
#include <sstream>
#include <BOOOS.h>
#include <Task.h>
#include "test_scaffold.h"


using namespace std;
using namespace BOOOS;

namespace Task_Test_Functions {
	int test_init() {
		BOOOS::BOOOS booos(false);

		ASSERT(Task::self()->tid() == 0, 1);
		ASSERT(Task::self()->state() == Task::RUNNING, 2);

		Task::init(); // should execute only once!
		ASSERT(Task::self()->tid() == 0, 3);
		ASSERT(Task::self()->state() == Task::RUNNING, 4);

		return 0;
	}

	void f1(void* a) {}
	void f2(void* a) {}
	void f3(void* a) {}
	void f4(void* a) {}

	int test_creation_destruction() {
		BOOOS::BOOOS booos(false);

		Task t1(f1, 1, (void*)"f1"), t2(f2, 1, (void*)"f2");
		Task * t3 = new Task(f3, 1, (void*)"f3");
		Task * t4 = new Task(f4, 1, (void*)"f4");

		ASSERT(t1.tid() == 2, 1);
		ASSERT(t1.state() == Task::READY, 2);
		ASSERT(t2.tid() == 3, 3);
		ASSERT(t2.state() == Task::READY, 4);
		ASSERT(t3->tid() == 4, 5);
		ASSERT(t3->state() == Task::READY, 6);
		ASSERT(t4->tid() == 5, 7);
		ASSERT(t4->state() == Task::READY, 8);

		delete t3;
		delete t4;

		return 0;
	}

	Task *ping, *pong;
	queue<string> log,correct;

	void f_ping(void * arg) {
		int i ;
		std::stringstream str1;
		str1 << (char *) arg << " Start" << endl;
		log.push(str1.str());
		str1.str().clear();

		for (i=0; i<4; i++)
		{
			std::stringstream str;
			str << "\t" << (char*) arg << " " << i << endl;
			log.push(str.str());
			ping->pass_to(pong);
		}
		std::stringstream str2;
		str2 << (char *) arg << " End" << endl;
		log.push(str2.str());

		ping->exit(0);
	}

	void f_pong(void * arg) {
		int i ;

		std::stringstream str1;
		str1 << (char *) arg << " Start" << endl;
		log.push(str1.str());
		str1.str().clear();

		for (i=0; i<4; i++)
		{
			std::stringstream str;
			str << "\t" << (char*) arg << " " << i << endl;
			log.push(str.str());
			str.str().clear();
			pong->pass_to(ping);
		}
		std::stringstream str2;
		str2 << (char *) arg << " End" << endl;
		log.push(str2.str());
		str2.str().clear();

		pong->exit(0);
	}

	int test_cooperative_execution()
	{

		BOOOS::BOOOS booos(false);

		correct.push("Main Start\n");
		correct.push("\tPing Start\n");
		correct.push("\t\tPing 0\n");
		correct.push("\tPong Start\n");
		correct.push("\t\tPong 0\n");
		correct.push("\t\tPing 1\n");
		correct.push("\t\tPong 1\n");
		correct.push("\t\tPing 2\n");
		correct.push("\t\tPong 2\n");
		correct.push("\t\tPing 3\n");
		correct.push("\t\tPong 3\n");
		correct.push("\tPing End\n");
		correct.push("\tPong End\n");
		correct.push("Main End\n");

		log.push("Main Start\n");

		ping = new Task(f_ping, 1, (char*)"\tPing");
		pong = new Task(f_pong, 1, (char*)"\tPong");

		Task::self()->pass_to(ping);
		Task::self()->pass_to(pong);

		log.push("Main End\n");

		ASSERT(log.size() == correct.size(), 1);

		string str1,str2;
		int i = 2;
		while(log.size()) {
			str1 = log.front(); log.pop();
			str2 = correct.front(); correct.pop();
			ASSERT(str1 == str2, i);
			i++;
		}

		return 0;
	}
}

int main() {
	cout << "Welcome to BOOOS - Basic Object Oriented Operating System!" << endl;
	cout << "This program will test the class: Task" << endl;

	UnitTest tests("Task");

	tests.attach_test(&Task_Test_Functions::test_init, "Init");
	tests.attach_test(&Task_Test_Functions::test_creation_destruction, "Creation and Destruction");
	tests.attach_test(&Task_Test_Functions::test_cooperative_execution, "Cooperative execution");

	tests.run();

	return 0;
}
