#include <list>
#include <mutex>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#include "audio.hpp"
#include "vorbis_ogg.cpp"

namespace audio{
SDL_AudioSpec output_spec;
SDL_AudioDeviceID deviceId;
std::list<sound> playing_sound;
std::mutex playing_sound_mtx;
std::vector<sound> loaded_sounds;
void queue(long unsigned index){
	playing_sound_mtx.lock();
	if(index < loaded_sounds.size())
	playing_sound.emplace_back(loaded_sounds[index]);
	playing_sound_mtx.unlock();
	SDL_PauseAudioDevice(deviceId, 0);
}
WAVE::~WAVE(){
		if(need_free)
			SDL_FreeWAV(buffer);
}
WAVE::WAVE(){need_free = 0;}

converter::converter(SDL_AudioSpec &from, SDL_AudioSpec &outp = output_spec) {
	out_spec = outp;
	stream = SDL_NewAudioStream(from.format, from.channels, from.freq,
								outp.format, outp.channels, outp.freq);
	if (stream == NULL) {
		printf("Uhoh, stream failed to create: %s\n", SDL_GetError());
	}
}
int converter::put(const void *buffer, int len) {  // Add len bytes from buffer
	int rc = SDL_AudioStreamPut(stream, buffer, len);
	if (rc) {
		printf("Uhoh, failed to put samples in stream: %s\n", SDL_GetError());
	}
	return rc;
}
int converter::avaliable() { return SDL_AudioStreamAvailable(stream); }
WAVE *converter::getall() {
	if (SDL_AudioStreamFlush(stream)) return new WAVE();
	return new WAVE(stream, out_spec);
}
converter::~converter() { SDL_FreeAudioStream(stream); }

sound::~sound() {
	//pcm.reset();
}
sound::sound(const WAVE &copy) {
	Uint8 *alloc;
	if (!(alloc = (Uint8 *)malloc(copy.length) )){
		fprintf(stderr, "Can't malloc\n");
	}
	pcm = std::make_shared<Uint8*>(alloc);

	length = copy.length;
	memcpy(*pcm, copy.buffer, copy.length);
	play_pos = *pcm;
	if (!(copy.spec.channels == output_spec.channels &&
		  copy.spec.format == output_spec.format &&
		  copy.spec.channels == output_spec.channels)) {
		fprintf(stderr, "Create sound with format different tham output_spec\n");
	}
	//printf("Copyed pcm to %p\n", *pcm);
}
sound::sound(const sound &cp) {
	pcm = cp.pcm;
	play_pos = *pcm;
	play_len = length = cp.length;
}
void sound::to_start() {
	play_len = length;
	play_pos = *pcm;
}

int init(){
	// Initialize SDL.
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		return -1;
	}
	SDL_AudioSpec want_spec;
	SDL_memset(&want_spec, 0, sizeof(want_spec));
	want_spec.freq = 48000;
	want_spec.format = AUDIO_S16LSB;
	want_spec.channels = 2;
	want_spec.samples = 4096;
	want_spec.callback = audio_callback;
	want_spec.userdata = NULL;

	//Log info
	/*int i = 1, count = SDL_GetNumAudioDevices(0);
	SDL_Log("Using audio driver: %s", SDL_GetCurrentAudioDriver());
	for (i = 0; i < count; ++i) {
		SDL_Log("Audio device %d: %s", i, SDL_GetAudioDeviceName(i, 0));
	}*/

	int i = 0;//open default device
	output_spec = want_spec;
	deviceId = SDL_OpenAudioDevice(SDL_GetAudioDeviceName(i, 0), 0, &want_spec, &output_spec, SDL_AUDIO_ALLOW_FREQUENCY_CHANGE);
	//SDL_Log("Using audio device: %s\n", SDL_GetAudioDeviceName(i, 0));
	if (deviceId <= 0) {
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		return -1;
	}
	SDL_Log("Audio out Freq %d, Format %hu\n", output_spec.freq, output_spec.format);
	//Unpause
	SDL_PauseAudioDevice(deviceId, 0);
	return 0;
}
void close(){
	// shut everything down
	SDL_CloseAudioDevice(deviceId);
}

audio::WAVE::WAVE(converter&& converted){
	*this = *converted.getall();
}
WAVE::WAVE(const void *mem, int len){
	success = SDL_LoadWAV_RW(SDL_RWFromConstMem(mem, len), 1, &this->spec, &this->buffer, &this->length) != NULL;
	if(!success)
		fprintf(stderr, "Could not open wav from %p : %s\n", mem, SDL_GetError());
	if(!(spec.channels == output_spec.channels && spec.format == output_spec.format && spec.channels == output_spec.channels)){
		converter conv(spec, output_spec);
		conv.put(buffer, (int)length);
		*this = *conv.getall();
	}
}
WAVE::WAVE(SDL_AudioStream *stream, SDL_AudioSpec &stream_spec){
	spec = stream_spec;
	length = (Uint32)SDL_AudioStreamAvailable(stream);
	success = 0;
	if((buffer = (Uint8*)malloc(length))){
		if (SDL_AudioStreamGet(stream, buffer, (int)length) == -1) {
			printf("Uhoh, failed to get converted data: %s\n", SDL_GetError());
		}else success = 1;
	}
}

// audio callback function fill stream with to_out bytes
void audio_callback(void *userdata, Uint8 *stream, int out) {
	(void) userdata;
	Uint32 to_out = (Uint32)out;
	memset(stream, 0, (size_t)to_out);
	Uint32 len = to_out;//length to copy of atual audio
	playing_sound_mtx.lock();
	for(std::list<sound>::iterator it = playing_sound.begin(); it != playing_sound.end(); ++it){
		Uint32 &play_len = it->play_len;	 // remaining len of *it audio
		Uint8 *&play_pos = it->play_pos;	 // reference to atual pos of *it audio
		//set len
		len = (to_out > play_len ? play_len : to_out);

		//mix the audio
		SDL_MixAudioFormat(stream, play_pos, output_spec.format, len, SDL_MIX_MAXVOLUME);

		play_pos += len;  // increment actual pos by len
		play_len -= len;  // decrement remaining time by len

		//remove from queue if ends
		if (to_out > play_len)
			it = playing_sound.erase(it);
	}
	playing_sound_mtx.unlock();
}
sound &create_sound(FILE * load){
	return *(new sound(WAVE(converter(ogg_read(load)))));
}
}
/*int play_sound() {
	if (audio::init()) return 1;
	// Queue sound
	audio::playing_sound.emplace_back(audio::sound(audio::WAVE(audio::converter(fopen("assets/audio/sfx_coin_double1.ogg", "r")))));
	return 0;
}*/
