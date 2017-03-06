#ifndef GAMESTREAM_CAPTURE
#define GAMESTREAM_CAPTURE
extern "C" {

#ifndef GS_XCB
#define GS_XCB
#include "xcb.h"
#endif

#include "avcodec.h"
#include "avformat.h"
#include "gstypes.hpp"
#include "libswscale/swscale.h"
#include "imgutils.h"
	
#include <stdlib.h>

#define ALIGN 32
}

#include <ctime>
#include "gstypes.hpp"

typedef struct{
	int width;
	int height;
} gs_screen_info;

class GSScreenCap {
protected:
	int curr_id=0;
public:
	AVFrame* getFrameFromPixmap(gs_image* img);
	virtual gs_image* captureFrame()=0;
	virtual gs_screen_info getScreenInfo()=0;
	virtual void freeImage(gs_image *img)=0;
};

#endif
