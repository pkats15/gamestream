#include <capture/gs_capture.hpp>
#include <chrono>
#include <iostream>
#include <thread>

extern "C" {
//#include <stdio.h>
#include "avformat.h"
#include <xcb.h>

#define FRAMES 600
}

// TODO REMOVE
#include "decode_test.hpp" //TESTING

#include "capture/gs_capture.hpp"
#include "encoder.hpp"
#include "gstypes.hpp"
#include "sched/gs_sched.hpp"

// Use X11 capturing class
#ifdef GS_USE_X11
#include "capture/x11_capture.hpp"
#endif


//            (╯°□°）╯︵ ┻━┻
//|-------------------------------------|TODOLIST|-------------------------------------|
// Accepted ticks: v, V, ✔, ✓
//|------------------------------------------------------------------------------------|
//            (╯°□°）╯︵ ┻━┻

using namespace std;

int main (int argc, char *argv[]) {
	this_thread::sleep_for(2s);
	X11ScreenCap cap(NULL, 0);
	GSFrameThread threads[FRAMES];
	AVFrame *frames[FRAMES] = {NULL};
	AVPacket *pkt[FRAMES];
	
	GSEncoder encoder((int)5*1e9, cap.getScreenInfo().width, cap.getScreenInfo().height, 60 ,AV_PIX_FMT_YUV420P); //Found optimal bitrate for 30 fps 1920x1080
	auto t0 = chrono::system_clock::now();
	for(int i=0; i<FRAMES+6; i++){
		if(i < FRAMES){
			threads[i] = GSFrameThread(&cap);
			threads[i].pollFrame();
		}	
		if(i > 5){
			//AVFrame *f = threads[i-6].getFrame();
			frames[i-6] = threads[i-6].getFrame();
			if(frames[i-6]->width != 1920){
				printf("ERR\n");
			}
		}
		this_thread::sleep_for(18ms);
	}
	auto t1 = chrono::system_clock::now();
	printf("TIME: %dms\n", chrono::duration_cast<chrono::milliseconds>(t1-t0));
	for(int i=0; i<FRAMES; i++){
		frames[i]->pts = i;
		encoder.encodeFrame(pkt, frames[i]);
		av_frame_free(&(frames[i]));
	}
	encoder.encodeFrame(pkt, NULL);
	decodePkts(pkt, encoder.context);
	
	
	/*
	//TODO Find a better way to support multiple libraries or systems
#ifdef GS_USE_X11
	cap = new X11ScreenCap(NULL, 0);
#endif
	
	GSEncoder encoder((int)1e7/3, cap->getScreenInfo().width, cap->getScreenInfo().height, 30,
					  AV_PIX_FMT_YUV420P); //Found optimal bitrate for 30 fps 1920x1080
	gs_image img;
	AVPacket *pkt[NUM_OF_FRAMES];
	int i;
	for (i = 0; i < NUM_OF_FRAMES; ++i) {
		img = cap->captureFrame();
		AVFrame *frame = encoder.getFrameFromPixmap(img);
		cap->freeImage(img);
		frame->pts = i;
		encoder.encodeFrame(pkt, frame);
		av_frame_free(&frame);
	}
	encoder.encodeFrame(pkt, NULL);
	//Encode test frame
	//TODO REMOVE
	//decodePkts(pkt, encoder.context); //TESTING*/
}
