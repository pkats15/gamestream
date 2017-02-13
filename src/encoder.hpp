
extern "C" {
#include "avcodec.h"
#include "avformat.h"
#include "gstypes.hpp"
}

class GSEncoder {
private:
	AVCodec *codec;
	AVCodecContext *context;
	AVFrame *frame;
	AVPacket *pkt;
public:
	GSEncoder(int bitrate, int width, int height, int framerate, AVPixelFormat format);
	int setFrameFromPixmap(gs_image img);
};