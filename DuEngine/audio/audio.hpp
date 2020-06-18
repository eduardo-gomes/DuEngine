#pragma once
#include <list>
#include <memory>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>
#ifndef _DU_AUDIO
#define _DU_AUDIO
#include "vorbis_ogg.hpp"

namespace audio{
// audio callback function fill stream with to_out bytes
void audio_callback(void *userdata, Uint8 *stream, int len);
//Output audio device specs set when device was opened
extern SDL_AudioSpec output_spec;
//Opened Device ID
extern SDL_AudioDeviceID deviceId;
//Struct to store PCM audio pointer and len
struct sound;
//Vector to store loaded sounds
extern std::vector<sound> loaded_sounds;
//Queue sound by index at vector loaded sounds
void queue(long unsigned);
//class to convert PCM data to another spec
class converter;
//Struct to hold PCM data and spec after load
struct WAVE{
	Uint32 length;
	Uint8 *buffer;
	SDL_AudioSpec spec;
	bool success, need_free;
	~WAVE();
	WAVE(converter&&);  // load wave from ogg need vorbis_ogg_file.cpp
	//WAVE(ogg_read&&); // load wave from ogg need vorbis_ogg_file.cpp
	WAVE(const void* mem, int len);//load wav from const mem and convert to output_spec if needed
	WAVE(SDL_AudioStream *stream, SDL_AudioSpec &spec);//Copy wave from audiostream
	WAVE();
	private:
};
class converter{
	SDL_AudioStream *stream;
	SDL_AudioSpec out_spec;
	public:
	converter(SDL_AudioSpec &from, SDL_AudioSpec &outp);
	converter(ogg_read&&);//create converter and pull all data from ogg_read need vorbis_audio
	int put(const void* buffer, int len);//put data to convert
	int avaliable();//avaliable data to get
	WAVE *getall();//get all data in WAVE
	~converter();
};
struct sound{
	Uint32 length, play_len;
	Uint8 *play_pos;
	std::shared_ptr<Uint8*> pcm;
	~sound();
	sound(const WAVE &copy);//create sound from WAVE
	sound(const sound &cp);//Copy sound and reset to start
	sound();
	sound& operator=(const sound&) = default;
	void to_start();
};
typedef unsigned int musicIndex;
//Create music
musicIndex music(unsigned int loaded_index);
//Set music state, -1 = stop, 0 = pause, 1 = play
void musicPlay(int state, musicIndex I);
//Reserve slots to create music
void musicReserve(unsigned int reserve);


extern std::list<sound> playing_sound;
//initialize SDL_AUDIO 
bool init();
//Close SDL_AUDIO
void close();

//Create sound from ogg file
//sound &create_sound(FILE *);
sound &create_sound(const std::string &filePath);

//sound *create_sound(const ogg_file&);
}
//Demo play sound
//int play_sound();
#endif
