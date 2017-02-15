
extern "C" {
#include "avcodec.h"
#include "avformat.h"
#include "gstypes.hpp"
#include "libswscale/swscale.h"
#include "imgutils.h"
}

#define ALIGN 32

class GSEncoder {
private:
	AVCodec *codec;
	AVCodecContext *context;
	void encode(AVFrame *frame, AVPacket *pkt);
public:
	GSEncoder(int bitrate, int width, int height, int framerate, AVPixelFormat format);
	AVFrame* getFrameFromPixmap(gs_image img);
	void encodeFrame(AVPacket *pkt, AVFrame *frame);
};