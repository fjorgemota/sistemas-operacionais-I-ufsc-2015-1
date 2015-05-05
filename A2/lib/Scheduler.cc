/* 
 * Scheduler.cc
 *
 *  Created on: Feb 27, 2014
 */

#include "Scheduler.h"
#include "Queue.h"
#include "BOOOS.h"
#include <stdlib.h>
#include <stdio.h>
#include <iostream>

using namespace std;


void print_queue(BOOOS::Queue *q);

namespace BOOOS
{


Scheduler * Scheduler::__dispatcher;

Scheduler::Scheduler() : Task(Scheduler::dispatcher, 0, 0)  {
    this->_state = Task::SCHEDULER;
	Task::__ready.remove(this); // Remove the scheduler from the queue
}

Scheduler::~Scheduler(){};

void Scheduler::dispatcher(void*) {
	while (Task::count() > 1) {
		if (Task::self()->state() == Task::RUNNING) {
			// If there is a task running actually, closes it. 
			// Example of such a situation: Such as when this context is selected automagically by uc_link
			Task::self()->exit(0);
			continue;
		}
		Task *next = Scheduler::self()->choose_next(); 	
		if (next) {
			if (BOOOS::SCHED_AGING == true and Task::__ready.length() > 0) {
				Task * elem = (Task*) Task::__ready.head()->next();
				do {
					elem->aging();
					elem = (Task*) elem->next();
				} while (elem != Task::__ready.head()->next());
			}
			Task::self()->pass_to(next, Task::READY);
		}
	} 
	Task::self()->exit(0);
}

void Scheduler::init() {
	if (Scheduler::__dispatcher != 0) {
		return; // If the dispatcher already exists, ignore it
	}
	Scheduler::__dispatcher = new Scheduler();
}

Task * Scheduler::choose_next() {
    return (Task*) Task::__ready.remove();
}
}
