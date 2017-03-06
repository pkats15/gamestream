#include "gs_sched.hpp"
using namespace std;

GSSchedUnit::GSSchedUnit (int sch_id, int width, int height, AVPixelFormat pix_fmt) {
	this->sch_id = sch_id;
	this->running = false;
	//this->cont = sws_getContext (width, height, AV_PIX_FMT_BGRA, width, height, pix_fmt, 0, NULL, NULL, NULL);
}

void GSSchedUnit::update (){
	chrono::time_point<chrono::system_clock, chrono::milliseconds> next_wake;// = chrono::high_resolution_clock::now ();
	while(running){
		//AVFrame *frame = cap->getFrameFromPixmap(cap->captureFrame());
		
	}
}

void GSSchedUnit::start () {
	running = true;
	thr = thread(&GSSchedUnit::update, this);
	thr.detach();
}

void GSSchedUnit::stop(){
	running = false;
}


GSFrameThread::GSFrameThread(GSScreenCap *cap){
	this->cap = cap;
	done = false;
}

GSFrameThread::GSFrameThread(){
	
}

void GSFrameThread::run(){
	auto a = chrono::high_resolution_clock::now();
	gs_image *img = cap->captureFrame();
	frame = cap->getFrameFromPixmap(img);
	cap->freeImage(img);
	auto b = chrono::high_resolution_clock::now();
	done = true;
}

void GSFrameThread::pollFrame(){
	thr = thread(&GSFrameThread::run, this);
}

AVFrame* GSFrameThread::getFrame(){
	thr.join();
	return frame;
}

