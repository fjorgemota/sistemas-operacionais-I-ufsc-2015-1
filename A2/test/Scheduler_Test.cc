/*
 * Queue_Test.cc
 *
 *  Created on: Feb 21, 2014
 */

#include <iostream>
#include <sstream>
#include <queue>
#include <BOOOS.h>
#include <Scheduler.h>
#include "test_scaffold.h"

using namespace std;
using namespace BOOOS;

namespace Scheduler_Test_Functions {

	int test_init() {
		BOOOS::BOOOS::SCHED_POLICY = BOOOS::BOOOS::SCHED_FCFS;
		BOOOS::BOOOS::SCHED_PREEMPT = false;
		BOOOS::BOOOS::SCHED_AGING = false;
		BOOOS::BOOOS booos(false);

		ASSERT(Scheduler::self()->tid() == 1, 1);
		ASSERT(Scheduler::self()->state() == Task::SCHEDULER, 2);
		ASSERT(Task::count() == 2, 3);

		Scheduler::init(); // should execute only once!
		ASSERT(Scheduler::self()->tid() == 1, 4);
		ASSERT(Scheduler::self()->state() == Task::SCHEDULER, 5);
		ASSERT(Task::count() == 2, 6);

		Task::self()->exit(0);

		return 0;
	}

	void f1(void* a) {}
	void f2(void* a) {}
	void f3(void* a) {}
	void f4(void* a) {}

	int test_creation_destruction() { // will add to Scheduler

		BOOOS::BOOOS::SCHED_POLICY = BOOOS::BOOOS::SCHED_FCFS;
		BOOOS::BOOOS::SCHED_PREEMPT = false;
		BOOOS::BOOOS::SCHED_AGING = false;
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
		ASSERT(Task::count() == 6, 9);

		delete t3;
		delete t4;

		Task::self()->exit(0);

		return 0;
	}


	queue<string> correct, log;

	void function(void * arg) {
		int i;

		for(i=0; i<4; i++) {
			std::stringstream str;
			str << (char*)arg << " " << i << endl;
			log.push(str.str());
			Task::self()->yield();
		}
		std::stringstream str;
		str << (char*)arg << " End" << endl;
		log.push(str.str());
		Task::self()->exit(0);
	}

	int test_scheduling() {

		/* Expected test output */
		correct.push("Main Start\n");
		correct.push("Main yielding...\n");
		correct.push("\tPang 0\n");
		correct.push("\t\tPeng 0\n");
		correct.push("\t\t\tPing 0\n");
		correct.push("\t\t\t\tPong 0\n");
		correct.push("\t\t\t\t\tPung 0\n");
		correct.push("Main yielding...\n");
		correct.push("\tPang 1\n");
		correct.push("\t\tPeng 1\n");
		correct.push("\t\t\tPing 1\n");
		correct.push("\t\t\t\tPong 1\n");
		correct.push("\t\t\t\t\tPung 1\n");
		correct.push("Main yielding...\n");
		correct.push("\tPang 2\n");
		correct.push("\t\tPeng 2\n");
		correct.push("\t\t\tPing 2\n");
		correct.push("\t\t\t\tPong 2\n");
		correct.push("\t\t\t\t\tPung 2\n");
		correct.push("Main yielding...\n");
		correct.push("\tPang 3\n");
		correct.push("\t\tPeng 3\n");
		correct.push("\t\t\tPing 3\n");
		correct.push("\t\t\t\tPong 3\n");
		correct.push("\t\t\t\t\tPung 3\n");
		correct.push("Main yielding...\n");
		correct.push("\tPang End\n");
		correct.push("\t\tPeng End\n");
		correct.push("\t\t\tPing End\n");
		correct.push("\t\t\t\tPong End\n");
		correct.push("\t\t\t\t\tPung End\n");
		correct.push("Main End\n");

		BOOOS::BOOOS::SCHED_POLICY = BOOOS::BOOOS::SCHED_FCFS;
		BOOOS::BOOOS::SCHED_PREEMPT = false;
		BOOOS::BOOOS::SCHED_AGING = false;
		BOOOS::BOOOS booos(false);

		log.push("Main Start\n");

		Task * pang = new Task(function, 1, (char*)"\tPang");
		Task * peng = new Task(function, 1, (char*)"\t\tPeng");
		Task * ping = new Task(function, 1, (char*)"\t\t\tPing");
		Task * pong = new Task(function, 1, (char*)"\t\t\t\tPong");
		Task * pung = new Task(function, 1, (char*)"\t\t\t\t\tPung");

		while(Task::count() > 2) {
			log.push("Main yielding...\n");
			Task::self()->yield();
		}

		delete pang;
		delete peng;
		delete ping;
		delete pong;
		delete pung;

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
	cout << "This program will test the class: Scheduler" << endl;

	UnitTest tests("Scheduler");

	tests.attach_test(&Scheduler_Test_Functions::test_init, "Init");
	tests.attach_test(&Scheduler_Test_Functions::test_creation_destruction, "Creation and Destruction");
	tests.attach_test(&Scheduler_Test_Functions::test_scheduling, "FCFS");

	tests.run();

	return 0;
}
