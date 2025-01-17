#pragma once
#include <iostream>

#define FRAMESIZE  4096 
#define DEFBUFSIZE 1024 

#define MAXPAGES 50000


struct bFrame
{
	char field[FRAMESIZE];
};


class BCB
{
public:
	BCB(int page, int frame) :page_id(page), frame_id(frame), dirty(0), next(0) {}
	BCB() :page_id(-1), frame_id(-1), dirty(0), next(0) {}
	~BCB() {
		next = 0;
	}
	int page_id;
	int frame_id;
	int latch;
	int count;
	int dirty;
	BCB* next;

};
