#include "gs_sched.hpp"

using namespace std;

//Compare frames using pts in order to add to queue
bool sortFrame::operator()(AVFrame *f1, AVFrame *f2) {
	//if (f1 != nullptr && f2 != nullptr) {
	return f1->pts < f2->pts;
	//}
	/*else {
		printf("NULL ERROR!\n");
		return true;
	}*/
}

GSSchedUnit::GSSchedUnit(int sch_id, GSScreenCap *cap, GSManager *man, int thread_num, int sleep_time) {
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

void GSSchedUnit::update() {
	chrono::time_point<chrono::high_resolution_clock> waitUntil = chrono::high_resolution_clock::now();
	waitUntil += chrono::milliseconds(sleep_time * sch_id);
	this_thread::sleep_until(waitUntil);
	while (running) {
		gs_image *img = cap->captureFrame();
		AVFrame *frame = cap->getFrameFromPixmap(img);
		cap->freeImage(img);
		frame->pts = iters * thread_num + sch_id;
		// printf ("THREAD #%d Frame: %d\n", sch_id, frame->pts);
		man->addToEncode(frame); // Add frame to queue for encoding
		iters++;
		waitUntil += chrono::milliseconds(sleep_time * thread_num);
		auto time_diff =
				chrono::duration_cast<chrono::milliseconds>(waitUntil - chrono::high_resolution_clock::now());
		if (time_diff.count() < 0) { // Skip next frame if the thread cannot handle it
			iters++;
			this_thread::sleep_until(waitUntil + chrono::milliseconds(sleep_time));
			// printf("Thread #%d Frame: %d Skipped\n", sch_id, frame->pts);
		} else {
			// printf("Sleep for %dms\n", time_diff);
			this_thread::sleep_until(waitUntil);
		}
	}
}

void GSSchedUnit::start() {
	running = true;
	thr = thread(&GSSchedUnit::update, this);
	// thr.detach();
}

GSSchedUnit::GSSchedUnit() {
	sch_id = -1; //Not properly initialized id
}

void GSSchedUnit::stop() {
	running = false;
}

void GSSchedUnit::end() {
	if (thr.joinable()) {
		thr.join();
	}
}

// Manager for GSSchedUnits
GSManager::GSManager(GSScreenCap *cap, GSEncoder *enc, int thread_num, int target_fps) {
	this->units = new GSSchedUnit[thread_num];
	this->thread_num = thread_num;
	this->enc = enc;
	int sleep_time = 1000 / target_fps;
	for (int i = 0; i < thread_num; i++) {
		units[i] = GSSchedUnit(i, cap, this, thread_num, sleep_time);
	}
}

void GSManager::start() {
	running = true;
	for (int i = 0; i < thread_num; i++) {
		units[i].start();
	}
	encode_thread = std::thread(&GSManager::encodeQueue, this);
}

void GSManager::stop() {
	running = false;    //Signal encoder thread to stop

	for (int i = 0; i < thread_num; i++) {
		units[i].stop();
	}
	for (int i = 0; i < thread_num; i++) {
		units[i].end();
	}

	encode_thread.join();

	// encodeQueue();

	// for (int i = 0; i < size; i++) {
	// 	//printf ("Encoding frame #%d|#%d\n", arr.front()->pts, i);
	// 	enc->encodeFrame (arr.front ());
	// 	av_frame_free(&arr.front());
	// 	arr.pop_front ();
	// }
	return;
}

void GSManager::encodeQueue() {
	// First lock then copy then free then clear then encode then unlock

	int last_pts = 0;  //The pts of the last encoded frame

	while (running) {
		// running = false;
		this_thread::sleep_for(20ms);
		int sz = (int) arr.size();
		if (sz == 0) {
			continue;
		}
		mut.lock();
		std::deque<AVFrame *> buf(arr.begin(), arr.end());
		arr.clear();
		mut.unlock();
		for (std::deque<AVFrame *>::iterator it = buf.begin(); it != buf.end(); it++) {
			AVFrame *f = *it;
//			                              //Very stupid way to solve this!
			if (f->pts > last_pts) {      //Does not encode skipped frames
				last_pts = (int) f->pts;  //TODO Fix
				printf("FRAME pts: %d\n", (int) f->pts);
				enc->encodeFrame(f);
				av_frame_free(&f);
				buf.pop_front();
			}
		}
	}
}


void GSManager::addToEncode(AVFrame *frame) {
	mut.lock();
	arr.push_back(frame);
	std::sort(arr.begin(), arr.end(), sortFrame());
	mut.unlock();
}
