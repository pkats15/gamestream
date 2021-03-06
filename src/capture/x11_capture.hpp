#ifndef GAMESTREAM_X11_CAPTURE
#define GAMESTREAM_X11_CAPTURE

#ifndef GAMESTREAM_CAPTURE
#include "gs_capture.hpp"
#endif

typedef struct gs_x11_image_t : gs_image{
	xcb_get_image_reply_t * reply;
} gs_x11_image;

class X11ScreenCap:public GSScreenCap{
private:
	xcb_connection_t *conn;
	xcb_screen_t *screen;
	xcb_drawable_t drawable;
	int screen_num;
	xcb_get_image_reply_t* last_rep; //TODO Use hashtable instead
public:
	//AVFrame* getFrameFromPixmap(gs_image img);
	X11ScreenCap(char *sever_name, int screen_num);
	gs_image *captureFrame();
	gs_screen_info getScreenInfo();
	void freeImage(gs_image* img);
};



#endif //GAMESTREAM_X11_CAPTURE
