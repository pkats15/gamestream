#ifndef GS_ENCODER
#define GS_ENCODER
extern "C" {
#include "avcodec.h"
#include "avformat.h"
#include "gstypes.hpp"
#include "libswscale/swscale.h"
#include "imgutils.h"
}

#include <mutex>

#define ALIGN 32

class GSEncoder {
private:
	std::mutex mut;
	AVCodec *codec;
	AVPacket **pkt;
public:
	AVCodecContext *context; //Moved from private to public for testing!
	GSEncoder(int bitrate, int width, int height, int framerate, AVPixelFormat format, AVPacket **pkt);
	AVFrame* getFrameFromPixmap(struct SwsContext *cont, gs_image img);
	void encodeFrame(AVFrame *frame);
};

#endif
