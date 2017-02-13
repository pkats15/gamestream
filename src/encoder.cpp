
#include <encoder.hpp>

GSEncoder::GSEncoder(int bitrate, int width, int height, int framerate, AVPixelFormat format){
	av_register_all();
	codec = avcodec_find_encoder(AV_CODEC_ID_H264);
	context = avcodec_alloc_context3(codec);
	frame = av_frame_alloc();
	pkt = av_packet_alloc();

	//Setup values
	context->bit_rate = bitrate;
	context->width = width;
	context->height = height;
	context->framerate = AVRational{1, framerate};
	context->time_base = AVRational{framerate, 1};
	context->max_b_frames=1;
	context->gop_size = 10;
	context->pix_fmt = format;
	context->profile = FF_PROFILE_H264_BASELINE;

	int err = avcodec_open2(context, codec, NULL);
	printf("%d\n", err);
}

int GSEncoder::setFrameFromPixmap(gs_image img) {
	//TODO Finish adding this function
	return 0;
}
