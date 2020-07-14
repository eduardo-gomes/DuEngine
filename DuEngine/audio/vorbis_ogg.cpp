#include "vorbis_ogg.hpp"

//#include <vorbis/codec.h>
//#include <vorbis/vorbisfile.h>
#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif
#include <cstring>

#include "manager/logger.hpp"
namespace audio {
DUENGINT int (*ov_open_callbacks)(void* datasource, OggVorbis_File* vf, const char* initial, long ibytes, ov_callbacks callbacks);
DUENGINT vorbis_info* (*ov_info)(OggVorbis_File* vf, int link);
DUENGINT ogg_int64_t (*ov_pcm_total)(OggVorbis_File* vf, int i);
DUENGINT vorbis_comment* (*ov_comment)(OggVorbis_File* vf, int link);
DUENGINT long (*ov_read)(OggVorbis_File* vf, char* buffer, int length, int bigendianp, int word, int sgned, int* bitstream);
DUENGINT int (*ov_clear)(OggVorbis_File* vf);

ogg_read::ogg_read(std::ifstream&& SrcFile) : sourceFile(std::move(SrcFile)) {
	valid = !ov_open_callbacks(&sourceFile, &file, nullptr, 0, ogg_callbacks::callbacks);
	if (!valid)
		fprintf(stderr, "Input does not appear to be an Ogg bitstream.\n");
	info = ov_info(&file, -1);
	auto ret = ov_pcm_total(&file, -1);
	if (ret < 0) throw std::runtime_error("ov_pcm_total < 0");
	samples = static_cast<uint64_t>(ret);
}
void ogg_read::disp_info() {
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
	snprintf(string, sizeof(string), "\nDecoded length: %lu samples\n", samples);
	logger::info(std::string(string));
	snprintf(string, sizeof(string), "Encoded by: %s\n\n", ov_comment(&file, -1)->vendor);
	logger::info(std::string(string));
	// END DISPLAY
}
int ogg_read::read(char* to, int len) {
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
ogg_read::~ogg_read() { ov_clear(&file); /*fclose((FILE*)file.datasource);*/ }

size_t ogg_read::ogg_callbacks::read(void* buffer, size_t size, size_t elementCount, void* dataSource) {
	if (size != 1) logger::erro("in ogg read callback size != 1");
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
	if (position < 0) throw std::out_of_range("in ogg tell callback position < 0");
	return static_cast<long>(position);
}

//load vorbisfile
void* handler;
#ifdef _WIN32
void DUENGINT* DLLOPEN(const char* filename) {
	void* handler = LoadLibraryA(filename);
	if (!handler) {
		DWORD error = GetLastError();
		logger::erro("Can't open libvorbisfile.so" + std::to_string(error));
		throw std::runtime_error("Can't load function : " + std::to_string(error));
	}
	return handler;
}
void DUENGINT* DLLLOADFUNC(void* handler, const char* name) {
	DWORD error;
	void* func = (void*)GetProcAddress((HMODULE)handler, name);
	if ((error = GetLastError()) != 0) {
		logger::erro("Can't resolve symbol libvorbisfile.so" + std::to_string(error));
		throw std::runtime_error("Can't load function : " + std::to_string(error));
	}
	return func;
}
void DUENGINT DLLCLOSE(void*& handler) {
	if (handler)
		FreeLibrary(static_cast<HMODULE>(handler));
	handler = nullptr;
}

#else
void DUENGINT* DLLOPEN(const char * filename){
	void* handler = dlopen(filename, RTLD_LAZY);
	if (!handler) {
		char* error = dlerror();
		logger::erro("Can't open libvorbisfile.so" + std::string(error));
		throw std::runtime_error("Can't load function : " + std::string(error));
	}
	return handler;
}
void DUENGINT* DLLLOADFUNC(void* handler, const char * name){
	char* error;
	void* func = dlsym(handler, name);
	if ((error = dlerror()) != NULL) {
		logger::erro("Can't resolve symbol libvorbisfile.so" + std::string(error));
		throw std::runtime_error("Can't load function : " + std::string(error));
	}
	return func;
}
void DUENGINT DLLCLOSE(void*& handler){
	if(handler)
		dlclose(handler);
	handler = nullptr;
}
#endif
int LoadVorbis() {
	//libvorbis is loaded by SDL
	try{
		handler = DLLOPEN("libvorbisfile.so");
		ov_open_callbacks = reinterpret_cast<int (*)(void*, OggVorbis_File*, const char*, long, ov_callbacks)>(DLLLOADFUNC(handler, "ov_open_callbacks"));
		ov_info = reinterpret_cast<vorbis_info(*(*)(OggVorbis_File*, int))>(DLLLOADFUNC(handler, "ov_info"));
		ov_pcm_total = reinterpret_cast<ogg_int64_t (*)(OggVorbis_File*, int)>(DLLLOADFUNC(handler, "ov_pcm_total"));
		ov_comment = reinterpret_cast<vorbis_comment* (*)(OggVorbis_File*, int)>(DLLLOADFUNC(handler, "ov_comment"));
		ov_read = reinterpret_cast<long (*)(OggVorbis_File*, char*, int, int, int, int, int*)>(DLLLOADFUNC(handler, "ov_read"));
		ov_clear = reinterpret_cast<int (*)(OggVorbis_File * vf)>(DLLLOADFUNC(handler, "ov_clear"));
	}
	catch(const std::runtime_error& e){
		logger::excp(e.what());
		return -1;
	}
	logger::info("Loaded libvorbisfile");
	return 0;
}
void CloseVorbis() {
	DLLCLOSE(handler);
}
}  // namespace audio