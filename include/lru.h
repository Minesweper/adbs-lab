#include "buffer_control.h"

class queu {
public:
	int id;
	queu* a;
	queu* b;

	queu(int id) :id(id) {

	}
	queu() :id(0) {

	}
	~queu() {
		a = 0, b = 0;
	}
};


class lru
{
public:
	lru(int bufsize);
	lru();
	~lru();
	int num();
	int capacity();
	queu* find(int d);
	int getfirst();
	void enqueu(int id);
	void dequeu();
	void update(int d);
	void remove(int d);
	bFrame page(int id) {
		return buf[id];
	}
	void writeframe(int id, bFrame f) {
		buf[id] = f;
	}
private:
	queu* first;
	queu* last;
	bFrame* buf;
	int _capacity;
	int size;
};

lru::lru(int bufsize) :_capacity(bufsize), size(0),first(0),last(0) {
	buf = new bFrame[bufsize];
}
lru::lru() :_capacity(DEFBUFSIZE), size(0),first(0),last(0) {
	buf = new bFrame[DEFBUFSIZE];
}
lru::~lru() {
	delete[] buf;
}
int lru::num() {
	return size;
}

int lru::capacity() {
	return _capacity;
}

int lru::getfirst() {
	if (size == 0) return -1;
	return first->id;
}
void lru::dequeu() {
	if (size == 0) return;
	if (size != 1) {
		queu* tmp = first;
		first = tmp->b;
		first->a = 0;
		delete tmp;
		size--;
	}
	else {
		delete first;
		first = last = 0;
		size = 0;
	}
}

void lru::enqueu(int id) {
	queu* tmp = new queu(id);
	if (size == 0) {
		first = last = tmp;
		first->b = last, last->a = first;
	}
	else {
		last->b = tmp;
		tmp->a = last;
		last = tmp;
	}
	size++;
}

queu* lru::find(int d) {
	if (size == 0) return 0;
	queu* tmp = first;
	while (tmp!=0)
	{
		if (tmp->id == d) return tmp;
		tmp = tmp->b;
	}
	return 0;
}

void lru::update(int d) {
	queu* tmp = find(d);
	if (tmp == 0) {
		if (_capacity == size) {
			dequeu();
			enqueu(d);
			return;
		}
		enqueu(d);
		return;
	}
	else if (tmp == first) {
		dequeu();
		enqueu(d);
		return;
	}
	else if (tmp == last) {
		return;
	}
	else {
		tmp->a->b = tmp->b;
		tmp->b->a = tmp->a;
		delete tmp;
		size--;
		enqueu(d);
		return;
	}
}

void lru::remove(int d) {
	if (size == 0) return;
	queu* tmp = find(d);
	if (tmp == 0) return;
	if (tmp == first) {
		dequeu();
		return;
	}
	tmp->a->b = tmp->b;
	tmp->b->a = tmp->a;
	delete tmp;
	size--;
}