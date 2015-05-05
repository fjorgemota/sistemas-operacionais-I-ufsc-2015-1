#ifndef QUEUE_CC_
#define QUEUE_CC_

#include "Queue.h"
#include <stdio.h>
#include <iostream>

using namespace BOOOS;
using namespace std;

void print_queue(Queue *q) {
	return;
	cout << "Queue length: " << q->length() << endl;

	if(q->length()) {
		int i;
		Queue::Element *elem = dynamic_cast<Queue::Element*>(q->head()->next());
		i = 0;
		do {
			cout << "TID:" << elem->tid() << " - Priority: " << elem->rank() << endl;
			elem = elem->next();
			i++;
		} while (elem != q->head()->next() && i < 10);
		cout << "In reverse order:" << endl;
		i = 0;
		elem = dynamic_cast<Queue::Element*>(q->head()->prev());
		do {
			cout << "TID:" << elem->tid() << " - Priority: " << elem->rank() << endl;
			elem = elem->prev();
			i++;
		} while (elem != q->head()->prev() && i < 10);
	}
	cout << "==============================" << endl;
}

Queue::Queue() {
	this->_length = 0;
}

Queue::~Queue() {
	while (this->_length) {
		this->remove();
	}
}

bool Queue::check(Element *elem) {
	return this->search(elem) != NULL;
}

void Queue::insert(Element * elem) {
	// // printf("==============================================================\n");
	// printf("Inserindo elemento: %d com rank %d\n", elem->tid(), elem->rank());
	if (this->_length == 0) {
		this->_head.next(elem);
		this->_head.prev(elem);
		elem->next(elem);
		elem->prev(elem);
	} else {
		// HEAD -> elem1 -> elem
		this->_head.prev()->next(elem);
		// HEAD -> elem1 -> elem
		elem->prev(this->_head.prev());
		// HEAD -> elem1 -> elem -> elem1
		elem->next(this->_head.next());
		this->_head.next()->prev(elem);
		this->_head.prev(elem);
	}
	this->_length++;
	// print_queue(this);
	// // printf("Terminou de inserir elemento...\n");
	// // printf("===============================================\n");
}

Queue::Element* Queue::remove() {
	if (this->_length == 0) {
		throw "ERROR"; // Empty queue
	}
	// // printf("==============================================================\n");
	Element* out = this->_head.next();
	// printf("Removendo elemento head->next() = %d com rank %d..\n", out->tid(), out->rank());
	if (this->_length == 1) {
		this->_head.next(0);
		this->_head.prev(0);
	} else {
		// out->next() é o novo 1º elemento
		Queue:Element *elem = out->next();
		// // printf("Proximo head->next() vai ser %d\n", elem->tid());
		// // printf("E o head->next()->next() vai ser %d\n", elem->next()->tid());
		this->_head.prev()->next(elem);
		this->_head.next(elem);
		elem->prev(this->_head.prev());
	}
	this->_length--;
	// print_queue(this);
	// // printf("Terminou de remover elemento head->next() = %d...\n", out->tid());
	// // printf("===============================================\n");
	return out;
}

void Queue::remove(Element * e) {
	// printf("Removendo elemento %d\n", e->tid());
	// // printf("==============================================================\n");
	// // printf("Removendo elemento %d..\n", e->tid());
	Element* sentinel = this->search(e);

	if (this->_length == 0 || sentinel == 0) {
		throw "ERROR";
	} else if (_length == 1) {
		this->remove();
		return;
	}

	Element* s_prev = sentinel->prev();
	Element* s_next = sentinel->next();
	if (sentinel == this->_head.prev()) {
		this->_head.prev(s_prev);
	} 
	if (sentinel == this->_head.next()) {
		this->_head.next(s_next);
	} 
	
	if (this->_length > 1) {
		s_prev->next(s_next);
		s_next->prev(s_prev);
	} else {
		this->_head.prev(0);
		this->_head.next(0);
	}
	this->_length--;
	// print_queue(this);
	// // printf("===============================================\n");
}

void Queue::insert_ordered(Element *elem) {
	if (this->_length == 0) {
		this->insert(elem);
		return;
	}
	// printf("Inserindo ordenadamente o elemento %d com rank %d\n", elem->tid(), elem->rank());
	Element* sentinel = _head.next();
	int count = 0;
	while (count < this->_length) {
		if (elem->rank() < sentinel->rank()) {
			break;
		}
		sentinel = sentinel->next();
		count++;
	}
	elem->next(sentinel);
	elem->prev(sentinel->prev());
	sentinel->prev()->next(elem);
	sentinel->prev(elem);
	if (count == 0) {
		this->_head.next(elem);
	} else if (count == this->_length) {
		this->_head.prev(elem);
	}
	this->_length++;
}

Queue::Element * Queue::search(Element * elem) {
	if (this->_length == 0) {
		return 0;
	}
	Element* sentinel = _head.next();
	do {
		// // printf("Checando se %d == %d\n",sentinel->tid(), elem->tid());
		if (sentinel == elem) {
			return sentinel;
		}
		sentinel = sentinel->next();
	} while (sentinel != this->head()->next());
	return 0;
}

#endif /* QUEUE_CC_ */