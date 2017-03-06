#ifndef GAMESTREAM_GS_SCHED
#define GAMESTREAM_GS_SCHED

#include <stdio.h>
#include "encoder.hpp"
#include "capture/gs_capture.hpp"
#include <thread>
#include "capture/x11_capture.hpp"
#include <iostream>


class GSSchedUnit {
private:
	struct SwsContext *cont;
	bool running;
	std::thread thr;
	GSScreenCap *cap;
public:
	void update();
	int sch_id;
	GSSchedUnit(int sch_id, int width, int height, AVPixelFormat pix_fmt);
	void start();
	void stop();
};

class GSFrameThread {
private:
	std::thread thr;
	GSScreenCap *cap;
	void run();
	AVFrame* frame;
	bool done;
public:
	GSFrameThread();
	GSFrameThread(GSScreenCap *cap);
	void pollFrame();
	AVFrame* getFrame();
};

#endif //GAMESTREAM_GS_SCHED
