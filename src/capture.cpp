#include "capture.hpp"

extern "C" {
#include <stdio.h>
}

ScreenRecorder::ScreenRecorder (int screen_num) {
	this->screen_num = screen_num;
	conn = xcb_connect (NULL, 0);
	const xcb_setup_t *setup = xcb_get_setup(conn);
	xcb_screen_iterator_t t= xcb_setup_roots_iterator(setup);
	screen = t.data;
	drawable = screen->root;
}
