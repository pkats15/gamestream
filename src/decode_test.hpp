
#ifndef GAMESTREAM_DECODE_TEST_HPP
#define GAMESTREAM_DECODE_TEST_HPP

extern "C"{
#include <avcodec.h>
#include <avformat.h>
#include <avio.h>
}
int decodePkts(AVPacket** pkts, AVCodecContext *context);
AVOutputFormat* getOFormatByName(char *name);

#endif //GAMESTREAM_DECODE_TEST_HPP
