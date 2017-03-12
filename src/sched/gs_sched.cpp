#include "gs_sched.hpp"
using namespace std;

int max_pts=0;

//Compare frames using pts in order to add to queue
bool sortFrame::operator() (AVFrame *f1, AVFrame *f2) {
//	if (f1 != nullptr && f2 != nullptr) {
	return f1->pts < f2->pts;
//	}
	/*else {
		printf("NULL ERROR!\n");
		return true;
	}*/
}

GSSchedUnit::GSSchedUnit (int sch_id, GSScreenCap *cap, GSManager *man, int thread_num, int sleep_time) {
	this->sch_id = sch_id;
	this->running = false;
	this->cap = cap;
	this->man = man;
	this->thread_num = thread_num;
	this->iters = 0; //Number of iterations
	this->sleep_time = sleep_time;
}

// GSSchedUnit::~GSSchedUnit(){

// }

void GSSchedUnit::update () {
	chrono::time_point<chrono::high_resolution_clock> waitUntil = chrono::high_resolution_clock::now ();
	waitUntil += chrono::milliseconds (sleep_time * sch_id);
	this_thread::sleep_until (waitUntil);
	while (running) {
		gs_image *img = cap->captureFrame ();
		AVFrame *frame = cap->getFrameFromPixmap (img);
		cap->freeImage(img);
		frame->pts = iters * thread_num + sch_id;
		//printf ("THREAD #%d Frame: %d\n", sch_id, frame->pts);
		man->addToEncode (frame); //Add frame to queue for encoding
		iters++;
		waitUntil += chrono::milliseconds (sleep_time * thread_num);
		auto time_diff = chrono::duration_cast<chrono::milliseconds>(waitUntil - chrono::high_resolution_clock::now());
		if(time_diff.count() < 0){ //Skip next frame if the thread cannot handle it
			iters++;
			this_thread::sleep_until(waitUntil+chrono::milliseconds(sleep_time));
			//printf("Thread #%d Frame: %d Skipped\n", sch_id, frame->pts);
		}else{
			//printf("Sleep for %dms\n", time_diff);
			this_thread::sleep_until (waitUntil);
		}
	}
}

void GSSchedUnit::start () {
	running = true;
	thr = thread (&GSSchedUnit::update, this);
	// thr.detach();
}

GSSchedUnit::GSSchedUnit () {
	sch_id = -1; //Not properly initialized id
}

void GSSchedUnit::stop () {
	running = false;
}

void GSSchedUnit::end () {
	if (thr.joinable ()) {
		thr.join ();
	}
}

//Manager for GSSchedUnits
//TODO Add encoding thread
GSManager::GSManager (GSScreenCap *cap, GSEncoder *enc, int thread_num, int target_fps) {
	this->units = new GSSchedUnit[thread_num];
	this->thread_num = thread_num;
	this->enc = enc;
	int sleep_time = 1000/target_fps;
	for (int i = 0; i < thread_num; i++) {
		units[i] = GSSchedUnit (i, cap, this, thread_num, sleep_time);
	}
}

void GSManager::start () {
	for (int i = 0; i < thread_num; i++) {
		units[i].start ();
	}
}

int GSManager::stop () {
	for (int i = 0; i < thread_num; i++) {
		units[i].stop ();
	}
	for (int i = 0; i < thread_num; i++) {
		units[i].end ();
	}
	int size = (int)arr.size();
	for (int i = 0; i < size; i++) {
		//printf ("Encoding frame #%d|#%d\n", arr.front()->pts, i);
		enc->encodeFrame (arr.front ());
		arr.pop_front ();
	}
	return size;
}

void GSManager::addToEncode (AVFrame *frame) {
	mut.lock();
	arr.push_back (frame);
	std::sort (arr.begin (), arr.end (), sortFrame ());
	mut.unlock();
}
