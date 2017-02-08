#define HEY "HEY"

extern "C" {
#include "xcb.h"
}

class ScreenRecorder {
private:
	unsigned int screen_num;
	xcb_connection_t *conn;
	xcb_screen_t *screen;
	xcb_drawable_t drawable;
public:
	ScreenRecorder (int screen_num);
};
