#include <iostream>
#include <chrono>

extern "C" {
//#include <stdio.h>
#include <xcb.h>
#include "avformat.h"
}

//TODO REMOVE
//#include "decode_test.hpp" //TESTING

#include "capture.hpp"
#include "encoder.hpp"
#include "gstypes.hpp"



//            (╯°□°）╯︵ ┻━┻
//|---------------------------------|TODOLIST|---------------------------------|
//Accepted ticks: v, V, ✔, ✓
//TODO Make a simple decoding test: |✓|
//|----------------------------------------------------------------------------|
//            (╯°□°）╯︵ ┻━┻

using namespace std;

int main(int argc, char *argv[]) {
	GSScreenCap cap(NULL, 0);
	GSEncoder encoder((int)1e7/3, cap.getScreen()->width_in_pixels, cap.getScreen()->height_in_pixels, 30,
					  AV_PIX_FMT_YUV420P); //Found optimal bitrate for 30 fps 1920x1080
	gs_image img;
	AVPacket *pkt[NUM_OF_FRAMES];
	auto p1 = chrono::high_resolution_clock::now();
	int i;
	for (i = 0; i < NUM_OF_FRAMES; ++i) {
		img = cap.captureFrame();
		AVFrame *frame = encoder.getFrameFromPixmap(img);
		cap.freeImage(img);
		frame->pts = i;
		encoder.encodeFrame(pkt, frame);
		av_frame_free(&frame);
	}
	auto p2 = chrono::high_resolution_clock::now();
	encoder.encodeFrame(pkt, NULL);
	//Encode test frame
	auto p3 = chrono::high_resolution_clock::now();
	printf("%f, %f\n", (p2 - p1) * (1e-6), (p3 - p1) * (1e-6));

	//TODO REMOVE
	//decodePkts(pkt, encoder.context); //TESTING
}
