#include "buffer_control.h"
#pragma once
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

