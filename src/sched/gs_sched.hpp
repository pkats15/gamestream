#ifndef GAMESTREAM_GS_SCHED
#define GAMESTREAM_GS_SCHED


#include <stdio.h>
#include "encoder.hpp"
#include "capture/gs_capture.hpp"
#include <thread>
#include "capture/x11_capture.hpp"
#include <iostream>
#include <algorithm>
#include <queue>
#include <mutex>

class GSManager;

class sortFrame {
public:
	bool operator()(AVFrame *f1, AVFrame *f2);
};

class GSSchedUnit {
private:
	bool running;
	std::thread thr;
	GSManager *man;
	GSScreenCap *cap;
	int iters;
	int thread_num;
	int sleep_time;
public:
	void update();
	int sch_id;
	GSSchedUnit(int sch_id, GSScreenCap *cap, GSManager *man, int thread_num, int sleep_time);
	GSSchedUnit();
	//~GSSchedUnit();
	void start();
	void stop();
	void end();
};

class GSManager {
private:
	std::mutex mut;
	GSEncoder *enc;
	GSSchedUnit *units;
	int thread_num;
	std::deque<AVFrame*> arr;
	void encodeQueue();
	bool running;
	std::thread encode_thread;
public:
	GSManager(GSScreenCap *cap, GSEncoder *enc, int thread_num, int target_fps);
	void start();
	void stop();
	void addToEncode(AVFrame *frame);
};

#endif //GAMESTREAM_GS_SCHED
