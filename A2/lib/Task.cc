/* Caique Rodrigues Marques (13204303) e Fernando Jorge Mota (13200641)
 * Task.cc
 *
 *  Created on: Feb 27, 2014
 */

#include "Task.h"
#include "BOOOS.h"
#include "Scheduler.h"
#include <stdlib.h>
#include <ucontext.h>
#include <stdio.h>
#include <Queue.h>

namespace BOOOS
{
volatile Task * Task::__running;
Task* Task::__main;
Queue Task::__ready;
int Task::STACK_SIZE = 32768;
int Task::__tid_counter = 1;
int Task::__task_counter = 0;

Task::Task(void (*entry_point)(void), int nargs, void * arg) {
	this->__task_counter++;
	this->_state = Task::READY;
	this->_stack = new char[Task::STACK_SIZE];
	this->_tid = Task::__tid_counter++;
	if (BOOOS::SCHED_POLICY == BOOOS::SCHED_FCFS) {
		Task::__ready.insert(this);
	} else if (BOOOS::SCHED_POLICY == BOOOS::SCHED_PRIORITY) {
		Task::__ready.insert_ordered(this);
	}
	this->nice(0);
	getcontext(&(this->context));
	this->context.uc_link          = (ucontext_t*) &(Scheduler::self()->context);
    this->context.uc_stack.ss_sp   = this->_stack;
    this->context.uc_stack.ss_size = Task::STACK_SIZE;
	makecontext(&(this->context), (void (*)(void)) entry_point, nargs, arg);

	if(BOOOS::SCHED_PREEMPT == true){
		if(this->rank() < Task::self()->rank()){
			Task::self()->yield();
		}
	}
}

Task::Task(void (*entry_point)(void*), int nargs, void * arg) {
	this->__task_counter++;
	this->_state = Task::READY;
	this->_stack = new char[Task::STACK_SIZE];
	this->_tid = Task::__tid_counter++;
	this->nice(0);
	if (BOOOS::SCHED_POLICY == BOOOS::SCHED_FCFS) {
		Task::__ready.insert(this);
	} else if (BOOOS::SCHED_POLICY == BOOOS::SCHED_PRIORITY) {
		Task::__ready.insert_ordered(this);
	}
	getcontext(&(this->context));
	this->context.uc_link          = (ucontext_t*) &(Scheduler::self()->context);
    this->context.uc_stack.ss_sp   = this->_stack;
    this->context.uc_stack.ss_size = Task::STACK_SIZE;
	makecontext(&(this->context), (void (*)(void)) entry_point, nargs, arg);

	if(BOOOS::SCHED_PREEMPT == true){
		if(this->rank() < Task::self()->rank()){
			Task::self()->yield();
		}
	}
}

Task::Task() {
	this->nice(0);
	this->__task_counter++;
	this->_state = Task::READY;
	this->_tid = 0;
	this->_stack = new char[Task::STACK_SIZE];
    this->context.uc_stack.ss_sp   = this->_stack;
    this->context.uc_stack.ss_size = Task::STACK_SIZE;
}

Task::~Task() {
	delete this->_stack;
	if (this->_state == Task::RUNNING) {
		this->exit(0); // If the task is already running, forces it to exit
	} else {
		if (this->_state == Task::READY and Task::__ready.check(this)) {
			Task::__task_counter--;
			// If the task is just ready it will be removed from the ready queue 
			// and decrements the task counter
			Task::__ready.remove(this); 
		}
	}
}

void Task::pass_to(Task *t, State s) {
	// Restore the original priority of the task BEFORE the context switch
	this->nice(this->rank()+this->_age);
	if (this->_state != Task::SCHEDULER) {
		this->_state = s;
	}
	Task::__running = t;
	if (t->_state != Task::SCHEDULER) {
		t->_state = Task::RUNNING;
	}
	if (this->_state == Task::READY) {
		// If the task is now ready, insert it into the ready queue
		if (BOOOS::SCHED_POLICY == BOOOS::SCHED_FCFS) {
			Task::__ready.insert(this);
		} else if (BOOOS::SCHED_POLICY == BOOOS::SCHED_PRIORITY) {
			Task::__ready.insert_ordered(this);
		}
	}
	swapcontext(&(this->context), &(t->context));
}

void Task::init() {
	Task::__tid_counter = 1;
	Task::__ready = Queue();
	Task::__task_counter = 0;
	Task::__main = new Task();
	Task::__running = Task::__main;
	Task::__running->_state = Task::RUNNING;
}

void Task::exit(int code) {
	Task::__task_counter--;
	if (this->_state == Task::READY and Task::__ready.check(this)) {
		// If the task is just ready it will be removed from the ready queue 
		// and decrements the task counter
		Task::__ready.remove(this); 
	} 
	this->pass_to( (Task*) ((this->_state == SCHEDULER) ? Task::__main : Scheduler::self()), Task::FINISHING);
}

void Task::yield() {
	this->pass_to(Scheduler::self(), Task::READY);
}

void Task::nice(int priority) {
	if(-20 > priority || priority > 20){ 
		return;
	}
	this->_age = 0;
	this->rank(priority);
}

void Task::aging() {
	this->rank(this->rank()-1);
	this->_age++;
}

} /* namespace BOOOS */
