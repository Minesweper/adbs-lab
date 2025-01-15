#include "../include/buffer_manager.h"

BMgr::BMgr(int bufs, std::string filename) :_capacity(bufs){
	ptof = new BCB * [bufs]();
	manager = new DSMgr(bufs, filename);
	lr = new lru(bufs);
}
BMgr::BMgr(std::string filename) :_capacity(DEFBUFSIZE) {
	ptof = new BCB * [DEFBUFSIZE]();
	manager = new DSMgr(DEFBUFSIZE, filename);
	lr = new lru(DEFBUFSIZE);
}
BMgr::~BMgr() {
	delete[] ptof;
	delete manager;
	delete lr;
}


NewPage BMgr::FixNewPage() {
	NewPage ans(-1, -1);
	if (lr->num() < _capacity) {
		int tmp = manager->newpage();
		if (tmp == -1) return ans;

	}
	
	return ans;
}

int BMgr::NumFreeFrames() {
	return _capacity - lr->num();
}

int BMgr::Hash(int page_id) {
	BCB* tmp = ptof[page_id % _capacity];
	while (tmp != 0) {
		if (tmp->page_id == page_id) return tmp->frame_id;
		tmp = tmp->next;
	}
	return -1;
}

BCB* BMgr::hash(int page_id) {
	BCB* tmp = ptof[page_id % _capacity];
	while (tmp != 0) {
		if (tmp->page_id == page_id) return tmp;
		tmp = tmp->next;
	}
	return 0;
}

void BMgr::SetDirty(int frame_id) {
	int d = manager->fratopage(frame_id);
	if (d != 0) {
		BCB* tmp = hash(d);
		if (tmp != 0)tmp->dirty = 1;
	}

}

void BMgr::UnsetDirty(int frame_id) {
	int d = manager->fratopage(frame_id);
	if (d != 0) {
		BCB* tmp = hash(d);
		if (tmp != 0)tmp->dirty = 0;
	}
}

void BMgr::PrintFrame(int frame_id) {
	int d = manager->fratopage(frame_id);
	printf("%s", manager->printpage(d));
}

void BMgr::RemoveLRUEle(int frid) {
	lr->remove(frid);
}

void BMgr::RemoveBCB(BCB* ptr, int page_id) {
	int d = page_id % _capacity;
	BCB* tmp = ptof[d];
	if (tmp == ptr) {
		ptof[d] = ptr->next;
	}
	else{
		while (tmp->next != ptr) tmp = tmp->next;
		tmp->next = ptr->next;
	}
	if (ptr->dirty) {
		manager->WritePage(ptr->frame_id, lr->page(ptr->frame_id));
	}
	delete ptr;
}

int BMgr::SelectVictim() {
	int d = lr->getfirst();
	int page = manager->fratopage(d);
	BCB* tmp = hash(page);
	RemoveBCB(tmp, page);
	return d;
}

void BMgr::WriteDirtys() {
	for (int i = 0; i < _capacity; i++) {
		int id = manager->fratopage(i);
		BCB* tmp = hash(id);
		if (tmp->dirty) {
			manager->WritePage(tmp->frame_id, lr->page(tmp->frame_id));
		}
	}
}

void BMgr::insertbcb(int page, int frame) {
	int i = page % _capacity;
	BCB* b = new BCB(page, frame);
	BCB* tmp = ptof[i];
	if (tmp == 0) ptof[i] = b;
	else{
		while (tmp->next != 0) tmp = tmp->next;
		tmp->next = b;
	}
	manager->sethash(frame, page);
}

void BMgr::writebuf(int page, int frame) {
	lr->writeframe(frame, manager->ReadPage(page));
	lr->enqueu(frame);
	insertbcb(page, frame);
}

int BMgr::FixPage(int page_id, int prot) {
	BCB* tmp = hash(page_id);
	int d;
	if (tmp == 0) {
		if (lr->num() == _capacity)
			d = SelectVictim();
		else d = lr->num();
		writebuf(page_id, d);
		return d;
	}
	else {
		lr->update(tmp->frame_id);
		if (prot) SetDirty(tmp->frame_id);
		return tmp->frame_id;
	}
}

int BMgr::UnfixPage(int page_id) {
	int d = page_id % _capacity;
	BCB* tmp = hash(d);
	while (tmp != 0) {
		if (tmp->page_id == page_id) {
			if (tmp->count == 0) {
				tmp->latch = 0;
				return tmp->frame_id;
			}
			else return -1;
		}
		tmp = tmp->next;
	}
	return -1;
}
