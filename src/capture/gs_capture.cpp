#include "gs_capture.hpp"



AVFrame* GSScreenCap::getFrameFromPixmap(gs_image *img) {

	// Convert image data to AVFrame format (data, linesizes)
	int linesize[4];
	av_image_fill_linesizes (linesize, AV_PIX_FMT_BGRA, img->width);
	const uint8_t *data[4] = { img->data };

	// Prepare frame
	AVFrame *frame = av_frame_alloc ();
	frame->width = img->width;
	frame->height = img->height;
	frame->format = AV_PIX_FMT_YUV420P;
	av_frame_get_buffer (frame, ALIGN);
	av_frame_make_writable (frame);

	//memcpy (frame->data, data, 4 * sizeof (uint8_t *));
	//memcpy (frame->linesize, linesize, 8 * sizeof (int));
	
	//TODO Find if av_image_alloc is necessary (possibly not at all)
	//Not needed since data is already allocated by av_frame_alloc()
	//av_image_alloc(frame->data, frame->linesize, img->width, img->height, AV_PIX_FMT_YUV420P, ALIGN);

	//Convert from BGRA (XY_PIXMAP) to YUV420P for encoding


	//auto start = std::chrono::high_resolution_clock::now();

	
	struct SwsContext *cont = sws_getContext(img->width, img->height, AV_PIX_FMT_BGRA, img->width, img->height, (AVPixelFormat)frame->format, 0, NULL, NULL, NULL);
	
	//auto mid = std::chrono::high_resolution_clock::now();
	sws_scale(cont, data, linesize, 0, img->height, frame->data, frame->linesize);
	//auto end = std::chrono::high_resolution_clock::now();
	//printf("%dms %dms enc\n", std::chrono::duration_cast<std::chrono::milliseconds>(mid - start), std::chrono::duration_cast<std::chrono::milliseconds>(end - mid));
	return frame;
}

/*
GSScreenCap::GSScreenCap () {
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

gs_image GSScreenCap::captureFrame(){
	xcb_get_image_cookie_t get_cookie;
	xcb_get_image_reply_t *get_reply;

	get_cookie = xcb_get_image(conn, XCB_IMAGE_FORMAT_Z_PIXMAP, drawable , 0, 0, screen->width_in_pixels, screen->height_in_pixels, (uint32_t) ~0);
	get_reply = xcb_get_image_reply(conn, get_cookie, NULL);
	return (gs_image){screen->width_in_pixels, screen->height_in_pixels, xcb_get_image_data(get_reply), get_reply};
}

xcb_screen_t *GSScreenCap::getScreen(){
	return screen;
}

void GSScreenCap::freeImage(gs_image img){
	free(img.reply);
}
*/
