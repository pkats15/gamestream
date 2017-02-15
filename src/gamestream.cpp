#include <iostream>

extern "C" {
#include <stdio.h>
#include <xcb.h>
}


#include "capture.hpp"
#include "encoder.hpp"



//            (╯°□°）╯︵ ┻━┻
//|---------------------------------|TODOLIST|---------------------------------|
//Accepted ticks: v, V, ✔, ✓
//TODO Finish adding setFrameFromPixmap function: |✓|
//TODO Fix packet encoding: |✓|
//|----------------------------------------------------------------------------|
//            (╯°□°）╯︵ ┻━┻

using namespace std;

int main (int argc, char *argv[]) {
	GSScreenCap cap (NULL, 0);
	gs_image img = cap.captureFrame();
	GSEncoder encoder (4000, cap.getScreen()->width_in_pixels, cap.getScreen()->height_in_pixels, 60, AV_PIX_FMT_YUV420P);

	//Encode test frame
	AVFrame *frame = encoder.getFrameFromPixmap(img);
	AVPacket *pkt = nullptr;
	encoder.encodeFrame(pkt, frame);
	encoder.encodeFrame(pkt, NULL);
}
