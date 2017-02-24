#ifndef GAMESTREAM_GSTYPES
#define GAMESTREAM_GSTYPES

#ifndef GS_XCB

#define GS_XCB
#include "xcb.h"
#endif //GS_XCB

//TODO Remove definition completely, use variable instead
#define NUM_OF_FRAMES 300

//Struct to hold image generated by gsscreencap
typedef struct {
	int width;
	int height;
	uint8_t *data;
	xcb_get_image_reply_t* reply;
} gs_image;
#endif //GAMESTREAM_GSTYPES
