#include <iostream>
#include <chrono>

extern "C" {
//#include <stdio.h>
#include <xcb.h>
}
#include "capture.hpp"
#include "encoder.hpp"
#include "gstypes.hpp"

#define NUM_OF_FRAMES 60


//            (╯°□°）╯︵ ┻━┻
//|---------------------------------|TODOLIST|---------------------------------|
//Accepted ticks: v, V, ✔, ✓
//TODO Fix packet allocation: |✓|
//|----------------------------------------------------------------------------|
//            (╯°□°）╯︵ ┻━┻

using namespace std;

int main(int argc, char *argv[]) {
	GSScreenCap cap(NULL, 0);
	GSEncoder encoder(4000, cap.getScreen()->width_in_pixels, cap.getScreen()->height_in_pixels, 60,
					  AV_PIX_FMT_YUV420P);
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
}
