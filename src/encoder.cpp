#include "gstypes.hpp"
#include <chrono>
#include <encoder.hpp>

GSEncoder::GSEncoder (int bitrate, int width, int height, int framerate, AVPixelFormat format) {
	av_register_all ();
	codec = avcodec_find_encoder (AV_CODEC_ID_H264);
	context = avcodec_alloc_context3 (codec);
	//	context->level=10;
	// Setup values
	context->bit_rate = bitrate;
	context->width = width;
	context->height = height;
	context->framerate = AVRational{ framerate, 1 };
	context->time_base = AVRational{ 1, framerate };
	context->max_b_frames = 1;
	context->gop_size = 10;
	context->pix_fmt = format;
	context->profile = FF_PROFILE_H264_HIGH;
	avcodec_open2 (context, codec, NULL);
}

AVFrame* GSEncoder::getFrameFromPixmap (struct SwsContext *cont, gs_image img) {

	// Convert image data to AVFrame format (data, linesizes)
	int linesize[4];
	av_image_fill_linesizes (linesize, AV_PIX_FMT_BGRA, img.width);
	const uint8_t *data[4] = { img.data };

	// Prepare frame
	AVFrame *frame = av_frame_alloc ();
	frame->width = img.width;
	frame->height = img.height;
	frame->format = context->pix_fmt;
	av_frame_get_buffer (frame, ALIGN);
	av_frame_make_writable (frame);

	//memcpy (frame->data, data, 4 * sizeof (uint8_t *));
	//memcpy (frame->linesize, linesize, 8 * sizeof (int));
	
	//TODO Find if av_image_alloc is necessary (possibly not at all)
	//av_image_alloc(frame->data, frame->linesize, img.width, img.height, context->pix_fmt, ALIGN);

	//Convert from BGRA (XY_PIXMAP) to YUV420P for encoding


	//auto start = std::chrono::high_resolution_clock::now();

	
	//struct SwsContext *cont = sws_getContext(img.width, img.height, AV_PIX_FMT_BGRA, img.width,
	//img.height, context->pix_fmt, 0, NULL, NULL, NULL);

	
	//auto mid = std::chrono::high_resolution_clock::now();
	sws_scale(cont, data, linesize, 0, img.height, frame->data, frame->linesize);
	//auto end = std::chrono::high_resolution_clock::now();
	//printf("%dms %dms enc\n", std::chrono::duration_cast<std::chrono::milliseconds>(mid - start), std::chrono::duration_cast<std::chrono::milliseconds>(end - mid));
	return frame;
}

// Copied from encode_video.c libav example
// URL: https://libav.org/documentation/doxygen/master/encode_video_8c-example.html
void GSEncoder::encodeFrame (AVPacket **pkt, AVFrame *frame) {
	AVPacket *tmp_pkt = av_packet_alloc ();
	int ret;
	/* send the frame to the encoder */
	ret = avcodec_send_frame (context, frame);
	if (ret < 0) {
		exit (1);
	}
	while (ret >= 0) {
		ret = avcodec_receive_packet (context, tmp_pkt);
		if (ret == AVERROR (EAGAIN) || ret == AVERROR_EOF) {
			return;
		} else if (ret < 0) {
			fprintf (stderr, "error during encoding\n");
			exit (1);
		}
		//		printf("encoded frame %3"PRId64" (size=%5d)\n", tmp_pkt->pts, tmp_pkt->size);
		// Array index is dts+1 since the first pkt dts is -1
		pkt[tmp_pkt->dts + 1] = av_packet_clone (tmp_pkt);
		//		av_packet_unref(pkt);
	}
}
