#include <cstring>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include "manager/logger.hpp"
#include "vorbis_ogg.hpp"
namespace audio{

ogg_read::ogg_read(std::ifstream &&SrcFile) : sourceFile(std::move(SrcFile)) {
	valid = !ov_open_callbacks(&sourceFile, &file, nullptr, 0, ogg_callbacks::callbacks);
	if (!valid)
		fprintf(stderr, "Input does not appear to be an Ogg bitstream.\n");
	info = ov_info(&file, -1);
	auto ret = ov_pcm_total(&file, -1);
	if(ret < 0) throw std::runtime_error("ov_pcm_total < 0");
	samples = static_cast<size_t>(ret);
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
	char string[256];
	snprintf(string, sizeof(string), "\nBitstream is %d channel, %ldHz\n", info->channels, info->rate);
	logger::info(std::string(string));
	snprintf(string, sizeof(string), "\nDecoded length: %ld samples\n", (int64_t)samples);
	logger::info(std::string(string));
	snprintf(string, sizeof(string), "Encoded by: %s\n\n", ov_comment(&file, -1)->vendor);
	logger::info(std::string(string));
	// END DISPLAY
}
int ogg_read::read(char *to, int len) {
	return ov_read(&file, to, len, 0, 2, 1,
				   &current_section);  // little endian signed 16bit pcm
}
SDL_AudioSpec ogg_read::getSpec() {
	SDL_AudioSpec Spec;
	Spec.freq = info->rate;
	Spec.format = AUDIO_S16LSB;
	Spec.channels = info->channels;
	return Spec;
}
ogg_read::~ogg_read() { ov_clear(&file); /*fclose((FILE*)file.datasource);*/}

size_t ogg_read::ogg_callbacks::read(void* buffer, size_t size, size_t elementCount, void* dataSource) {
	if(size != 1) logger::erro("in ogg read callback size != 1");
	std::ifstream& stream = *static_cast<std::ifstream*>(dataSource);
	stream.read(static_cast<char*>(buffer), static_cast<std::streamsize>(elementCount));
	const std::streamsize bytesRead = stream.gcount();
	stream.clear();	 // In case we read past EOF
	return static_cast<size_t>(bytesRead);
}

int ogg_read::ogg_callbacks::seek(void* dataSource, ogg_int64_t offset, int origin) {
	static constexpr std::array<std::ios_base::seekdir, 3> seekDirections{
		std::ios_base::beg, std::ios_base::cur, std::ios_base::end};

	std::ifstream& stream = *static_cast<std::ifstream*>(dataSource);
	stream.seekg(offset, seekDirections.at(static_cast<size_t>(origin)));
	stream.clear();	 // In case we seeked to EOF
	return 0;
}

long ogg_read::ogg_callbacks::tell(void* dataSource) {
	std::ifstream& stream = *static_cast<std::ifstream*>(dataSource);
	const auto position = stream.tellg();
	if(position < 0) throw std::out_of_range("in ogg tell callback position < 0");
	return static_cast<long>(position);
}
}