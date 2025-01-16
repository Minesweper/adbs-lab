#include "../include/storage_manager.h"

DSMgr::DSMgr(int bufsize, std::string filename) :size(0), numPages(0), rn(0), wn(0) {
	OpenFile(filename);
	ftop = new int[bufsize]();
}
DSMgr::DSMgr(int bufsize) :size(0), numPages(0), rn(0), wn(0) {
	ftop = new int[bufsize]();
}
DSMgr::~DSMgr() {
	if(currFile!=0) CloseFile();
	delete[] ftop;
}
FILE* DSMgr::GetFile() {
	return currFile;
}
int DSMgr::GetNumPages() {
	return numPages;
}
void DSMgr::SetUse(int index, int use_bit) {
	pages[index] = use_bit;
}

int DSMgr::GetUse(int index) {
	return pages[index];
}

int DSMgr::OpenFile(std::string filename) {
	if (currFile != 0) return 1;
	currFile = fopen(filename.c_str(), "a+");
	if (currFile == 0) return 1;
	fseek(currFile, 0, SEEK_END);
	long long tmp = ftell(currFile);
	numPages = tmp / FRAMESIZE;
	size = tmp;
	return 0;
}

int DSMgr::CloseFile() {
	return fclose(currFile);
}

int DSMgr::Seek(int offset, int pos) {
	return fseek(currFile, offset, pos);
}

void DSMgr::IncNumPages() {
	numPages++;
}
int DSMgr::WritePage(int frame_id, bFrame frm) {
	int d = ftop[frame_id];
	fseek(currFile, d * FRAMESIZE, 0);
	SetUse(d, 0);
	fwrite(frm.field, FRAMESIZE, 1, currFile);
	wn++;
	return 0;
}
bFrame DSMgr::ReadPage(int page_id) {
	char tmp[FRAMESIZE];
	fseek(currFile, page_id * FRAMESIZE, 0);
	fread(tmp, FRAMESIZE, 1, currFile);
	rn++;
	bFrame ans;
	ans.field = tmp;
	SetUse(page_id, 1);
	return ans;
}

char* DSMgr::PrintPage(int page_id) {
	return ReadPage(page_id).field;
}

int DSMgr::NewPage() {
	int i;
	for (i = 0; i < numPages; i++) {
		if (pages[i] == 0) {
			pages[i] = 1;
			return i;
		}
	}
	return -1;
}