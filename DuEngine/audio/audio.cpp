#include "audio.hpp"

#include <DuEngine/manager/logger.hpp>
//#include "vorbis_ogg.cpp"

namespace audio {
std::unique_ptr<audio> audioOut;
audio::audio() {
	LOGDEBUG("AUDIO Constructor");
	if (LoadVorbis() < 0) {
		throw std::runtime_error("Falied LoadVorbis");
	}
	SDL_AudioSpec want;
	if (SDL_InitSubSystem(SDL_INIT_AUDIO) < 0) {
		logger::erro("Couldn't open audio: " + std::string(SDL_GetError()));
	}

	SDL_memset(&want, 0, sizeof(want));
	want.freq = 48000;
	want.format = AUDIO_S16LSB;
	want.channels = 2;
	want.samples = 4096;
	want.callback = audio_callback;
	want.userdata = this;

	int DevCount = SDL_GetNumAudioDevices(0);
	for (int i = 0; i < DevCount; ++i) {
		std::string DevName;
		DevName = SDL_GetAudioDeviceName(i, 0);
		logger::info("Audio device Avaliable " + std::to_string(i) + " : " + DevName);
	}

	dev = SDL_OpenAudioDevice(NULL, 0, &want, &outputSpec, SDL_AUDIO_ALLOW_ANY_CHANGE);
	if (dev == 0) {
		std::string err("Failed to open audio: ");
		err += SDL_GetError();
		logger::erro(err);
	} else {
		SDL_PauseAudioDevice(dev, 0); /* start audio playing. */
		logger::info("Opened Audio Device");
		logger::info("Audio Info: freq " + std::to_string(outputSpec.freq));
		logger::info("Audio Info: format " + std::to_string(outputSpec.format));
		logger::info("Audio Info: channels " + std::to_string(outputSpec.channels));
		logger::info("Audio Info: samples " + std::to_string(outputSpec.samples));
	}
}
audio::~audio() {
	SDL_CloseAudioDevice(dev);
	SDL_AudioQuit();
	logger::info("Closed Audio Device");
	CloseVorbis();
}
const SDL_AudioSpec audio::getSpec() const {
	return outputSpec;
}
WAVE::WAVE(size_t length) : length(length) {
	avaliable.store(0, std::memory_order_relaxed);
	PCM = malloc(length);
}
WAVE::~WAVE() {
	free((void*)PCM);
}
const std::shared_ptr<WAVE>& converter::getWAVE() const {
	return wave;
}
converter::converter(const SDL_AudioSpec& from, size_t samples, const SDL_AudioSpec& to) {
	stream = SDL_NewAudioStream(from.format, from.channels, from.freq, to.format, to.channels, to.freq);
	if (stream == NULL) {
		logger::erro(std::string("Uhoh, stream failed to create: ") += SDL_GetError());
	}
	size_t size = static_cast<size_t>(to.channels) * (to.format & SDL_AUDIO_MASK_BITSIZE) / 8 * samples;
	wave = std::make_shared<WAVE>(size);
}
inline int converter::getAvaliable() {
	int avail = SDL_AudioStreamAvailable(stream);
	size_t avaliable = wave->avaliable.load(std::memory_order_relaxed);	 //////////mudar o nome disso
	int64_t canStore = static_cast<int64_t>(wave->length - avaliable);
	if (avail > canStore) {
		//logger::info("More Avaliable Than can store");
		avail = canStore;
	}
	void* out = (wave->PCM + avaliable);
	int gotten = SDL_AudioStreamGet(stream, out, avail);
	if (gotten == -1) {
		logger::erro(std::string("Uhoh, failed to get converted data: ") += SDL_GetError());
	} else {
		wave->avaliable.fetch_add(static_cast<size_t>(gotten), std::memory_order_release);
	}
	return gotten;
}
int converter::flush() {
	int fl = SDL_AudioStreamFlush(stream);
	if (fl == -1) {
		logger::erro(std::string("Uhoh, failed to flush data: ") += SDL_GetError());
		return fl;
	} else {
		return getAvaliable();
	}
}
int converter::put(void* from, size_t len = 4096) {
	// you tell it the number of _bytes_, not samples, you're putting!
	int rc = SDL_AudioStreamPut(stream, from, len);
	if (rc == -1) {
		logger::erro(std::string("Uhoh, failed to put samples in stream: ") += SDL_GetError());
		return rc;
	}
	getAvaliable();
	(void)len;
	return rc;
}
converter::~converter() {
	SDL_FreeAudioStream(stream);
}
void audio::EnqueueSound(const std::shared_ptr<WAVE>& src) {
	SoundsQueue.emplace_back(sound(src));
}
audio::music& audio::EnqueueMusic(const std::shared_ptr<WAVE>& src) {
	MusicQueue.emplace_back(music(src));
	return MusicQueue.back();
}
audio::sound::sound(const std::shared_ptr<WAVE>& src) : wave(src), played(0) {
	pos = wave->PCM;
}
audio::music::music(const std::shared_ptr<WAVE>& src) : wave(src), played(0) {
	pos = wave->PCM;
	status = 1;
}
void audio::music::SetStatus(int toStatus) {
	if (toStatus < 0)
		status = -1;
	else
		status = toStatus;
}

// audio callback function fill stream with to_out bytes
void audio_callback(void* userdata, Uint8* stream, int outlen) {
	audio& out = *(static_cast<audio*>(userdata));
	uint32_t to_out = static_cast<uint32_t>(outlen);
	memset(stream, 0, (size_t)to_out);
	uint32_t len = to_out;	//length to copy of atual audio
	for (std::list<audio::music>::iterator it = out.MusicQueue.begin(); it != out.MusicQueue.end(); ++it) {
		if (it->status != 1) continue;
		size_t& played = it->played;																					// played len of *it audio
		const uint32_t play_len = static_cast<uint32_t>(it->wave->avaliable.load(std::memory_order_relaxed) - played);	// remaining len of *it audio
		void*& play_pos = it->pos;																						// reference to atual pos of *it audio
		//set len
		len = (to_out > play_len ? play_len : to_out);
		//mix the audio
		SDL_MixAudioFormat(stream, (Uint8*)play_pos, out.outputSpec.format, len, SDL_MIX_MAXVOLUME);

		play_pos = play_pos + len;	// increment actual pos by len
		played += len;				// increment played time by len

		//rewind if ends
		if (to_out > play_len) {
			it->played = 0;
			it->pos = it->wave->PCM;
		}
	}
	for (std::list<audio::sound>::iterator it = out.SoundsQueue.begin(); it != out.SoundsQueue.end(); ++it) {
		size_t& played = it->played;																					// played len of *it audio
		const uint32_t play_len = static_cast<uint32_t>(it->wave->avaliable.load(std::memory_order_relaxed) - played);	// remaining len of *it audio
		void*& play_pos = it->pos;																						// reference to atual pos of *it audio
		//set len
		len = (to_out > play_len ? play_len : to_out);

		//mix the audio
		SDL_MixAudioFormat(stream, (Uint8*)play_pos, out.outputSpec.format, len, SDL_MIX_MAXVOLUME);

		play_pos = play_pos + len;	// increment actual pos by len
		played += len;				// increment played time by len

		//remove from queue if ends
		if (to_out > play_len)
			it = out.SoundsQueue.erase(it);
	}
}

}  // namespace audio