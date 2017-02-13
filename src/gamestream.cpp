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
//TODO Finish adding setFrameFromPixmap function: ||
//|----------------------------------------------------------------------------|
//            (╯°□°）╯︵ ┻━┻

using namespace std;

int main (int argc, char *argv[]) {
	GSScreenCap cap (NULL, 0);
	gs_image img = cap.captureFrame();
	GSEncoder encoder (4000, cap.getScreen()->width_in_pixels, cap.getScreen()->height_in_pixels, 60, AV_PIX_FMT_YUV420P);
}
