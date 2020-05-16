//#include <vorbis/codec.h>
//#include <vorbis/vorbisfile.h>

//#include <cstring>
#pragma once

//extern "C" char _binary_music_ogg_start;
//extern "C" char _binary_music_ogg_end;
struct ogg_file {  // read from memory
	char *curPtr;
	char *filePtr;
	unsigned long fileSize;
};
struct ogg_read;
