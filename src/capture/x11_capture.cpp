#include "x11_capture.hpp"
#include <stdio.h>

X11ScreenCap::X11ScreenCap(char *server_name, int screen_num) {
	this->screen_num = screen_num;
	conn = xcb_connect(NULL, 0);
	const xcb_setup_t *setup = xcb_get_setup(conn);
	xcb_screen_iterator_t t = xcb_setup_roots_iterator(setup);
	for (int i = 0; i < screen_num; i++) {
		xcb_screen_next(&t);
	}
	screen = t.data;
	drawable = screen->root;
}

gs_image* X11ScreenCap::captureFrame() {
	xcb_get_image_cookie_t get_cookie;
	xcb_get_image_reply_t *get_reply;
	get_cookie = xcb_get_image(conn, XCB_IMAGE_FORMAT_Z_PIXMAP, drawable, 0, 0, screen->width_in_pixels, screen->height_in_pixels, (uint32_t) ~0);
	get_reply = xcb_get_image_reply(conn, get_cookie, NULL);
	last_rep = get_reply;
	gs_x11_image *img = new gs_x11_image;
	img->width=screen->width_in_pixels;
	img->height=screen->height_in_pixels;
	img->data=xcb_get_image_data(get_reply);
	img->id=curr_id++;
	img->reply=get_reply;
	//	printf("%d\n", get_reply);
	return img;
}

gs_screen_info X11ScreenCap::getScreenInfo(){
	return gs_screen_info{screen->width_in_pixels, screen->height_in_pixels};
}

void X11ScreenCap::freeImage(gs_image* img) {
	//	printf("%X\n", ((gs_x11_image*)img)->reply);
	delete ((gs_x11_image*)img)->reply;
}
