#include <iostream>
#include <chrono>
#include <capture/gs_capture.hpp>

extern "C" {
//#include <stdio.h>
#include <xcb.h>
#include "avformat.h"
}

//TODO REMOVE
//#include "decode_test.hpp" //TESTING

#include "gstypes.hpp"
#include "encoder.hpp"
#include "capture/gs_capture.hpp"

//Use X11 capturing class
#ifdef GS_USE_X11
#include "capture/x11_capture.hpp"
#endif


//            (╯°□°）╯︵ ┻━┻
//|-------------------------------------|TODOLIST|-------------------------------------|
//Accepted ticks: v, V, ✔, ✓
//TODO Add inheritance to capture.cpp/.hpp and move to separate directory: |✓|
//|------------------------------------------------------------------------------------|
//            (╯°□°）╯︵ ┻━┻

using namespace std;

int main(int argc, char *argv[]) {
	GSScreenCap *cap;

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
	//decodePkts(pkt, encoder.context); //TESTING
}
