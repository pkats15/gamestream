extern "C" {

#ifndef GS_XCB
#define GS_XCB
#include "xcb.h"
#endif

#include <stdlib.h>
}

#include <ctime>
#include "gstypes.hpp"

class GSScreenCap {
private:
	xcb_connection_t *conn;
	xcb_screen_t *screen;
	xcb_drawable_t drawable;
	int screen_num;

public:
	GSScreenCap (char *server_name, int screen_num);
	gs_image captureFrame ();
	xcb_screen_t* getScreen();
	void freeImage(gs_image img);
};
