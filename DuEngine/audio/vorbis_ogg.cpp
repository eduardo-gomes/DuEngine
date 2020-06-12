#include <cstring>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include "vorbis_ogg.hpp"
struct ogg_read {
	OggVorbis_File file;
	vorbis_info *info;
	ogg_int64_t samples;
	int current_section;
	bool valid;
	ogg_read(const ogg_file &open);	 // Create reader from ogg_file
	ogg_read(FILE * from_disk);	//Create reader from ogg_file in disk
	int read(char *to, int len); //try to read len bytes, return bytes readed
	long readall(char *to);	//read all data
	void disp_info();
	~ogg_read();
};

#ifdef _WIN32
ov_callbacks ovcallback = {
	/*read_func =*/ [](void *ptr, size_t size, size_t nmemb, void *datasource) -> size_t {
		ogg_file* of = reinterpret_cast<ogg_file*>(datasource);
		size_t len = size * nmemb;
		if (of->curPtr + len > of->filePtr + of->fileSize) {
			len = (size_t)(of->filePtr + of->fileSize - of->curPtr);
		}
		memcpy(ptr, of->curPtr, len);
		of->curPtr += len;
		return len;
	},
	/*seek_func =*/ [](void *datasource, ogg_int64_t offset, int whence) -> int {
		ogg_file *of = reinterpret_cast<ogg_file *>(datasource);

		switch (whence) {
			case SEEK_CUR:
				of->curPtr += offset;
				break;
			case SEEK_END:
				of->curPtr = of->filePtr + of->fileSize - offset;
				break;
			case SEEK_SET:
				of->curPtr = of->filePtr + offset;
				break;
			default:
				return -1;
		}
		if (of->curPtr < of->filePtr) {
			of->curPtr = of->filePtr;
			return -1;
		}
		if (of->curPtr > of->filePtr + of->fileSize) {
			of->curPtr = of->filePtr + of->fileSize;
			return -1;
		}
		return 0;
	},
	/*close_func =*/ [](void *) -> int { return 0; },
	/*tell_func =*/ [](void *datasource) -> long {
		ogg_file *of = reinterpret_cast<ogg_file *>(datasource);
		return (of->curPtr - of->filePtr);
	},
};
#endif
ogg_read::ogg_read(const ogg_file &open) {
#ifdef _WIN32
	valid = !ov_open_callbacks(&open, &file, NULL, -1, ovcallback);
#else
	FILE *openedfile = fmemopen(open.filePtr, open.fileSize, "r");
	valid = !ov_open(openedfile, &file, NULL, -1);
#endif

	if (!valid)
		fprintf(stderr, "Input does not appear to be an Ogg bitstream.\n");
	info = ov_info(&file, -1);
	samples = ov_pcm_total(&file, -1);
}
ogg_read::ogg_read(FILE* to_open) {
	valid = !ov_open(to_open, &file, NULL, -1);
	if (!valid)
		fprintf(stderr, "Input does not appear to be an Ogg bitstream.\n");
	info = ov_info(&file, -1);
	samples = ov_pcm_total(&file, -1);
}
void ogg_read::disp_info(){
	// DISPLAY
	//coments
	/*fprintf(stderr, "user_comments\n");
	char **ptr = ov_comment(&file, -1)->user_comments;
	while (*ptr) {
		fprintf(stderr, "%s\n", *ptr);
		++ptr;
	}*/
	//info
	fprintf(stderr, "\nBitstream is %d channel, %ldHz\n", info->channels, info->rate);
	fprintf(stderr, "\nDecoded length: %ld samples\n", (long)samples);
	fprintf(stderr, "Encoded by: %s\n\n", ov_comment(&file, -1)->vendor);
	// END DISPLAY
}
int ogg_read::read(char *to, int len) {
	return ov_read(&file, to, len, 0, 2, 1,
				   &current_section);  // little endian signed 16bit pcm
}
long ogg_read::readall(char *to) {
	int ret = -1;
	long readed = 0;
	while (ret) {
		ret = ov_read(&file, to + readed, 4096, 0, 2, 1, &current_section);  // little endian signed 16bit pcm
		if(ret > 0)
			readed += ret;
		//printf("readed %d\n", ret);
	}
	return readed;
}
ogg_read::~ogg_read() { ov_clear(&file); /*fclose((FILE*)file.datasource);*/}

#ifdef _DU_AUDIO
audio::converter::converter(ogg_read&& read){
	out_spec = output_spec;
	stream = SDL_NewAudioStream(AUDIO_S16LSB, read.info->channels, read.info->rate,
								out_spec.format, out_spec.channels, out_spec.freq);
	if (stream == NULL) {
		printf("Uhoh, stream failed to create: %s\n", SDL_GetError());
	}
	int ogg_rd = -1;
	char bufferl[4096];
	while (ogg_rd) {
		ogg_rd = read.read(bufferl, 4096);
		if (ogg_rd > 0) put(bufferl, ogg_rd);
	}

}
#endif