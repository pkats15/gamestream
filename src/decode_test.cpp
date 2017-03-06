
#include <iostream>
#include "decode_test.hpp"
#include <avio.h>
#include "gstypes.hpp"
#include "avutil.h"

using namespace std;

AVOutputFormat* getOFormatByName(char *name){
	AVOutputFormat *of = NULL;
	while(true){
		of= av_oformat_next(of);
		if(!strcmp(of->name, "mp4"))
			return of;
	}
	return NULL;
}

int decodePkts(AVPacket **pkts, AVCodecContext *context){
	AVFormatContext *fc;
	AVOutputFormat *of;
	avformat_alloc_output_context2(&fc, NULL, "mp4", "file.mp4");
	avio_open2(&(fc->pb), "file.mp4", AVIO_FLAG_WRITE, NULL, NULL);
	of = fc->oformat;
//	AVCodecContext *c;
	AVCodec *codec = avcodec_find_encoder(of->video_codec);
	AVStream *stream = avformat_new_stream(fc, codec);
	stream->id = 0;//fc->nb_streams;
//	if (fc->oformat->flags & AVFMT_GLOBALHEADER)
//		context->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;

	stream->index=0;
	avcodec_parameters_from_context(stream->codecpar, context);
	avformat_write_header(fc,  NULL);
	for (int i = 0; i < NUM_OF_FRAMES; ++i) {
		pkts[i]->stream_index=0;
		//Rescale
		pkts[i]->dts = av_rescale_q(pkts[i]->dts, context->time_base, stream->time_base);
		pkts[i]->pts = av_rescale_q(pkts[i]->pts, context->time_base, stream->time_base);
		av_write_frame(fc, pkts[i]);
	}
	av_write_frame(fc, NULL);
	av_write_trailer(fc);
	return 0;
}
