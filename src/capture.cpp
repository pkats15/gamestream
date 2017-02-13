#include "capture.hpp"

// TODO compile in split down frame
// TODO test CLion

extern "C" {
#include <stdio.h>
}

ScreenRecorder::ScreenRecorder (char *server_name, int screen_num) {
	this->screen_num = screen_num;
	conn = xcb_connect (NULL, 0);
	const xcb_setup_t *setup = xcb_get_setup (conn);
	xcb_screen_iterator_t t = xcb_setup_roots_iterator (setup);
	for (int i = 0; i < screen_num; i++) {
		xcb_screen_next (&t);
	}
	screen = t.data;
	drawable = screen->root;
}

gs_image ScreenRecorder::captureFrame(){
	xcb_get_image_cookie_t get_cookie;
	xcb_get_image_reply_t *get_reply;
	get_cookie = xcb_get_image(conn, XCB_IMAGE_FORMAT_Z_PIXMAP, drawable , 0, 0, screen->width_in_pixels, screen->height_in_pixels, (uint32_t) ~0);
	get_reply = xcb_get_image_reply(conn, get_cookie, NULL);
	return (gs_image){screen->width_in_pixels, screen->height_in_pixels, xcb_get_image_data(get_reply)};
}
