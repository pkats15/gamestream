#include <capture/gs_capture.hpp>
#include <chrono>
#include <iostream>
#include <thread>


#include "decode_test.hpp" //TESTING

#include "encoder.hpp"
#include "sched/gs_sched.hpp"

// Use X11 capturing class
#ifdef GS_USE_X11
#include "capture/x11_capture.hpp"
#endif


//            (╯°□°）╯︵ ┻━┻
//|-------------------------------------|TODOLIST|-------------------------------------|
// Accepted ticks: v, V, ✔, ✓
//TODO Find why it crashes / SEGFAULTS: |✓|
//TODO Add frame skipping and thread synchronization: |✓|
//TODO Add encoding thread: |✓|
//|------------------------------------------------------------------------------------|
//            (╯°□°）╯︵ ┻━┻

using namespace std;

int main (int argc, char *argv[]) {
	AVPacket *pkt[10000]= {0};
	X11ScreenCap cap (NULL, 0);
	GSEncoder encoder (500000000, cap.getScreenInfo().width, cap.getScreenInfo().height, 60, AV_PIX_FMT_YUV420P, pkt); // Found optimal bitrate for 30 fps 1920x1080
	GSManager man(&cap, &encoder, 6, 60);
	man.start();
	this_thread::sleep_for(5s);
	int num_of_frames = man.stop();
	decodePkts(pkt, encoder.context, num_of_frames);
	
	// this_thread::sleep_for(2s);

	/*	GSFrameThread threads[FRAMES];
		AVFrame *frames[FRAMES] = { NULL };
		AVPacket *pkt[FRAMES];
		auto t0 = chrono::system_clock::now ();
		for (int i = 0; i < 10; i++) {
			gs_image *img = cap.captureFrame ();
			this_thread::sleep_for(5ms);
			AVFrame *frame = cap.getFrameFromPixmap (img);
			frame->pts = i;
			// cap.freeImage(img);
			encoder.encodeFrame (pkt, frame);
		}
		// av_frame_free(&frame);
		encoder.encodeFrame (pkt, NULL);
		auto t1 = chrono::system_clock::now ();
		printf ("%dms\n", chrono::duration_cast<chrono::milliseconds> (t1 - t0));
		// decodePkts(pkt, encoder.context);

		*/

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
