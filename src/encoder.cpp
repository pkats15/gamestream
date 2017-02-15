
#include <encoder.hpp>
#include "gstypes.hpp"

GSEncoder::GSEncoder(int bitrate, int width, int height, int framerate, AVPixelFormat format){
	av_register_all();
	codec = avcodec_find_encoder(AV_CODEC_ID_H264);
	context = avcodec_alloc_context3(codec);

	//Setup values
	context->bit_rate = bitrate;
	context->width = width;
	context->height = height;
	context->framerate = AVRational {1, framerate};
	context->time_base = AVRational {framerate, 1};
	context->max_b_frames=1;
	context->gop_size = 10;
	context->pix_fmt = format;
//	context->profile = FF_PROFILE_H264_BASELINE;

	int err = avcodec_open2(context, codec, NULL);
	printf("%d\n", err);
}

AVFrame* GSEncoder::getFrameFromPixmap(gs_image img) {
	int linesize[4];
	av_image_fill_linesizes(linesize, AV_PIX_FMT_BGRA, img.width);
	const uint8_t * data[4] = {img.data};
	AVFrame *frame = av_frame_alloc();
	frame->width = img.width;
	frame->height = img.height;
	frame->format = context->pix_fmt;
	av_frame_get_buffer(frame, ALIGN);
	av_frame_make_writable(frame);
	av_image_alloc(frame->data, frame->linesize, img.width, img.height, context->pix_fmt, ALIGN);
	struct SwsContext *cont = sws_getContext(img.width, img.height, AV_PIX_FMT_BGRA, img.width, img.height, context->pix_fmt, 0, NULL, NULL, NULL);\
	sws_scale(cont, data, linesize, 0, img.height, frame->data, frame->linesize);
	//TODO TEMP
	frame->pts = 0;
	return frame;
}

void GSEncoder::encodeFrame(AVPacket *pkt, AVFrame *frame) {
	if (pkt == nullptr) {
		pkt = av_packet_alloc();
	}
	encode(frame, pkt);
}

//Copied from encode_video.c libav example
//URL: https://libav.org/documentation/doxygen/master/encode_video_8c-example.html
void GSEncoder::encode(AVFrame *frame, AVPacket *pkt) {
	int ret;
	/* send the frame to the encoder */
	ret = avcodec_send_frame(context, frame);
	if (ret < 0) {
		exit(1);
	}
	while (ret >= 0) {
		ret = avcodec_receive_packet(context, pkt);
		if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
			return;
		}
		else if (ret < 0) {
			fprintf(stderr, "error during encoding\n");
			exit(1);
		}
		printf("encoded frame %3"PRId64" (size=%5d)\n", pkt->pts, pkt->size);
		av_packet_unref(pkt);
	}
}
