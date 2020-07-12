#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_audio.h>

#include <atomic>
#include <list>
#include <memory>

#include <DuEngine/visibility.hpp>
#include "vorbis_ogg.hpp"

namespace audio {
class DUENGINT WAVE {
   protected:
	size_t length;

   public:
	std::atomic<size_t> avaliable;
	WAVE(size_t);
	~WAVE();
	void* PCM;
	friend class converter;
	friend void audio_callback(void* userdata, Uint8* stream, int out);
};
class DUENGINT converter {
	std::shared_ptr<WAVE> wave;
	SDL_AudioStream* stream;

   public:
	int put(void* from, size_t len);
	int getAvaliable();
	int flush();
	const std::shared_ptr<WAVE>& getWAVE() const;
	converter(const SDL_AudioSpec& from, size_t samples, const SDL_AudioSpec& to);
	~converter();
};
class DUENGEXT audio {
	SDL_AudioSpec outputSpec;
	SDL_AudioDeviceID dev;

   public:
	class sound {
		std::shared_ptr<WAVE> wave;
		size_t played;
		void* pos;

	   public:
		sound(const std::shared_ptr<WAVE>&);
		friend void audio_callback(void* userdata, Uint8* stream, int out);
	};
	class music {
		std::shared_ptr<WAVE> wave;
		size_t played;
		void* pos;
		int status;

	   public:
		void SetStatus(int status);
		music(const std::shared_ptr<WAVE>&);
		friend void audio_callback(void* userdata, Uint8* stream, int out);
	};

   private:
	std::list<sound> SoundsQueue;
	std::list<music> MusicQueue;

   public:
	void EnqueueSound(const std::shared_ptr<WAVE>&);
	music& EnqueueMusic(const std::shared_ptr<WAVE>&);
	const SDL_AudioSpec getSpec() const;
	audio();
	~audio();
	friend void audio_callback(void* userdata, Uint8* stream, int out);
};
extern DUENGEXT std::unique_ptr<audio> audioOut;
// audio callback function fill stream with to_out bytes
void DUENGINT audio_callback(void* audioClass, Uint8* stream, int len);
}  // namespace audio
