//#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>

#include <fstream>

#pragma once
#include "audio.hpp"
namespace audio {
struct ogg_read {
	std::ifstream sourceFile;
	OggVorbis_File file;
	vorbis_info* info;
	size_t samples;
	int current_section;
	bool valid;
	ogg_read(std::ifstream&& file);	 //Create reader from ogg_file in disk
	int read(char* to, int len);	 //try to read len bytes, return bytes readed
	void disp_info();
	SDL_AudioSpec getSpec();
	~ogg_read();
	struct ogg_callbacks {
		static size_t read(void* ptr, size_t size, size_t nmemb, void* datasource);
		static int seek(void* dataSource, int64_t offset, int origin);
		static long tell(void* dataSource);
		static constexpr ov_callbacks callbacks{read, seek, nullptr, tell};
	};
};
}  // namespace audio
