/*
 * BOOOS.h
 *
 *  Created on: Aug 14, 2014
 */

#include <iostream>
#include <Queue.h>
#include "test_scaffold.h"


using namespace std;
using namespace BOOOS;

class MyElement : public Queue::Element {
public:
	MyElement(string str) : _name(str) {}

	virtual ~MyElement() {}

	string & name() { return _name; }

private:
	string _name;
};

void print_queue(Queue & q) {
	cout << "Queue length: " << q.length() << endl;

	if(q.length()) {
		MyElement * elem = dynamic_cast<MyElement *>(q.head()->next());
		do {
			cout << elem->name() << endl;
			elem = dynamic_cast<MyElement *>(elem->next());
		} while (elem != q.head()->next());
	}

	cout << "==============================" << endl;
}

int test_creation_destruction() {
	Queue *q = new Queue();

	ASSERT(q->length() == 0, 1);
	ASSERT(q->head()->next() == 0, 2);
	ASSERT(q->head()->prev() == 0, 3);

	delete q;

	return 0;
}

int test_insertion() {
	Queue q;

	MyElement * person1 = new MyElement("João");
	MyElement * person2 = new MyElement("Pedro");
	MyElement * person3 = new MyElement("Augusto");
	MyElement * person4 = new MyElement("Fábio");

	q.insert(person1);
	q.insert(person2);
	q.insert(person3);
	q.insert(person4);

	ASSERT(q.head()->next() == person1, 1); // head
	ASSERT(q.head()->prev() == person4, 2); // tail

	MyElement * tmp = dynamic_cast<MyElement*>(q.head()->next());
	ASSERT(tmp == person1, 3);
	tmp = dynamic_cast<MyElement*>(tmp->next());
	ASSERT(tmp == person2, 4);
	tmp = dynamic_cast<MyElement*>(tmp->next());
	ASSERT(tmp == person3, 5);
	tmp = dynamic_cast<MyElement*>(tmp->next());
	ASSERT(tmp == person4, 6);

	return 0;
}

int test_remotion() {
	Queue q;

	MyElement * person1 = new MyElement("João");
	MyElement * person2 = new MyElement("Pedro");
	MyElement * person3 = new MyElement("Augusto");
	MyElement * person4 = new MyElement("Fábio");

	q.insert(person1);
	q.insert(person2);
	q.insert(person3);
	q.insert(person4);

	MyElement * removed_person = dynamic_cast<MyElement *>(q.remove());
	ASSERT(person1 == removed_person, 1);
	delete removed_person;

	ASSERT(q.head()->next() == person2, 2);
	ASSERT(person2->prev() == q.head()->prev(), 3);
	ASSERT(q.head()->prev()->next() == person2, 4);

	return 0;
}

int main() {
	cout << "This program will test the class: Queue" << endl;

	UnitTest tests("Queue");

	tests.attach_test(&test_creation_destruction, "Creation and Destruction");
	tests.attach_test(&test_insertion, "Insertion");
	tests.attach_test(&test_remotion, "Remotion");

	tests.run();

	return 0;
}
