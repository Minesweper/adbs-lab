#include "buffer_control.h"
#include "lru.h"
#include "storage_manager.h"


class NewPage {
public:
	int pageid;
	int frameid;
	NewPage(int a, int b) :pageid(a), frameid(b) {}
};


class BMgr
{
public:
	BMgr(int bufsize, std::string filename);
	BMgr(std::string filename);
	~BMgr();
	int FixPage(int page_id, int prot);
	NewPage FixNewPage();
	int UnfixPage(int page_id);
	int NumFreeFrames();
	int SelectVictim();
	int Hash(int page_id);
	BCB* hash(int page_id);
	void RemoveBCB(BCB* ptr, int page_id);
	void RemoveLRUEle(int frid);
	void SetDirty(int frame_id);
	void UnsetDirty(int frame_id);
	void WriteDirtys();
	void PrintFrame(int frame_id);
	void writebuf(int page, int frame);
	void insertbcb(int page, int frame);
	int ReadNum(){
		return manager->ReadNum();
	}
	int WriteNum(){
		return manager->WriteNum();
	}
	double hitrate(){
		return (double)(bufferin) / (double)(bufferin + bufferout);
	}
private:
	int _capacity;
	int bufferin;
	int bufferout;
	DSMgr* manager;
	BCB** ptof;
	lru* lr;
};
