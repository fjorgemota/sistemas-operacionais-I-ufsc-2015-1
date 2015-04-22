/* Fernando Jorge Mota (13200641) e Caique Rodrigues Marques (13204303)
 * Task.cc
 *
 *  Created on: Feb 27, 2014
 */

#include "Task.h"
#include <stdlib.h>
#include <ucontext.h>
#include <stdio.h>
namespace BOOOS
{

volatile Task * Task::__running;
Task* Task::__main;
int Task::STACK_SIZE = 32768;
int Task::__tid_counter = 1;

Task::Task(void (*entry_point)(void), int nargs, void * arg) {
	this->_state = Task::READY;
	this->_stack = new char[Task::STACK_SIZE];
	this->_tid = ++Task::__tid_counter;
	getcontext(&(this->context));
	this->context.uc_link          = (ucontext_t*) &(Task::__running->context);
    this->context.uc_stack.ss_sp   = this->_stack;
    this->context.uc_stack.ss_size = Task::STACK_SIZE;
	makecontext(&(this->context), (void (*)(void)) entry_point, nargs, arg);
}

Task::Task(void (*entry_point)(void*), int nargs, void * arg) {
	this->_state = Task::READY;
	this->_stack = new char[Task::STACK_SIZE];
	this->_tid = ++Task::__tid_counter;
	getcontext(&(this->context));
	this->context.uc_link          = (ucontext_t*) &(Task::__running->context);
    this->context.uc_stack.ss_sp   = this->_stack;
    this->context.uc_stack.ss_size = Task::STACK_SIZE;
	makecontext(&(this->context), (void (*)(void)) entry_point, nargs, arg);
}

Task::Task() {
	this->_state = Task::READY;
	this->_stack = new char[Task::STACK_SIZE];
    this->context.uc_stack.ss_sp   = this->_stack;
    this->context.uc_stack.ss_size = Task::STACK_SIZE;
	this->_tid = 0;
}

Task::~Task() {
	delete this->_stack;
}

void Task::pass_to(Task *t, State s) {
	this->_state = s;
	Task::__running = t;
	t->_state = Task::RUNNING;
	swapcontext(&(this->context), &(t->context));
}

void Task::init() {
	Task::__tid_counter = 1;
	Task::__main = new Task();
	Task::__running = Task::__main;
	Task::__running->_state = Task::RUNNING;
}

void Task::exit(int code) {
	this->pass_to(Task::__main, Task::FINISHING);
}

} /* namespace BOOOS */
