extern "C" {
	#include "xcb.h"
	#include "libavutil/avutil.h"
}
#include <ctime>

typedef struct {
	int width;
	int height;
	uint8_t *data;
} gs_image;

class ScreenRecorder {
private:
	xcb_connection_t *conn;
	xcb_screen_t *screen;
	xcb_drawable_t drawable;
	int screen_num;
public:
	ScreenRecorder(char *server_name, int screen_num);
	gs_image captureFrame();
};
