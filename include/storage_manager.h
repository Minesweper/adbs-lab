#pragma once

#include <cstring>
#include <string>
#include "buffer_control.h"


class DSMgr
{
public:
	DSMgr(int bufsize, std::string filename);
	DSMgr(int bufsize);
	~DSMgr();
	int OpenFile(std::string filename);
	int CloseFile();
	bFrame ReadPage(int page_id);
	char* PrintPage(int page_id);
	int WritePage(int frame_id, bFrame frm);
	int Seek(int offset, int pos);
	FILE* GetFile();
	void IncNumPages();
	int GetNumPages();
	void SetUse(int index, int use_bit);
	int GetUse(int index);
	int FraToPage(int frameid) {
		return ftop[frameid];
	}
	void SetHash(int f, int p) {
		
		ftop[f] = p;
	}
	int NewPage();
	int ReadNum(){
		return rn;
	}
	int WriteNum(){
		return wn;
	}
private:
	int* ftop;
	FILE* currFile;
	long long size;
	int numPages;
	int pages[MAXPAGES];
	int rn;
    int wn;
};