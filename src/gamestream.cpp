#include <iostream>

extern "C" {
#include <stdio.h>
#include <xcb.h>
}


#include "capture.hpp"

using namespace std;

int main (int argc, char *argv[]) {
	ScreenRecorder rec (NULL, 0);
	gs_image img = rec.captureFrame();
}
