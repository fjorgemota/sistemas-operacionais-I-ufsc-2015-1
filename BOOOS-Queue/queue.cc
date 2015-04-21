#include "queue.h"
#include <stdlib.h>

using namespace BOOOS;

Queue::Queue() {
  this->_length = 0;
}

Queue::~Queue() {
	while (this->_length) {
		this->remove();
	}
}

void Queue::insert(Queue::Element * elem) {
	Queue::Element *h = this->head();
	if (this->_length) {
		Queue::Element *hprevious = h->prev();
		hprevious->next(elem);
		elem->next(h->next());
		elem->prev(hprevious);
		h->prev(elem);
	} else {
		h->next(elem);
		h->prev(elem);
		elem->next(elem);
		elem->prev(elem);
	}
	this->_length++;
}

Queue::Element * Queue::remove() {
	if (this->_length == 0) {
		throw "Cannot remove from the file";
	}
	Queue::Element *h = this->head();
	Queue::Element *result = h->next();
	--this->_length;
	h->next(result->next());
	if (this->_length) {
		h->prev()->next(result->next());
		result->next()->prev(h->prev());
	}
	return result;
}