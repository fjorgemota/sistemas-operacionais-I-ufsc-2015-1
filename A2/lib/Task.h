/*
 * Task.h
 *
 *  Created on: Aug 15, 2014
 */

#ifndef TASK_H_
#define TASK_H_

#include "Queue.h"
#include <ucontext.h>

namespace BOOOS {

class Task : public Queue::Element {
public:
	enum State {
		READY,
		WAITING,
		RUNNING,
		FINISHING,
		SCHEDULER
	};
	
	Task(void (*entry_point)(void), int nargs, void * arg);

	Task(void (*entry_point)(void*), int nargs, void * arg);
	virtual ~Task();

	virtual int tid() { return _tid; }

	State state() { return _state; }

	void pass_to(Task * t, State s = READY);

	void exit(int code);

	void yield();

	void nice(int priority);

        void aging();

	static Task * self() { return (Task*) __running; }
	static Queue ready() { return __ready; } 
	static int count() { return Task::__task_counter; }
	static void init();

private:

	Task();

	static volatile Task * __running;
	static int STACK_SIZE;
	static int __tid_counter;
	static int __task_counter;

	static Task* __main;

	char* _stack;
	ucontext_t context;
	
	int _tid; // task ID

	int _age;
	// ...

protected:
    static Queue __ready;
	State _state;
};

} /* namespace BOOOS */

#endif /* TASK_H_ */
