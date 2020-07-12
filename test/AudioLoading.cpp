#include <DuEngine/DuEngine.hpp>
#include <chrono>
#include <future>

namespace scene {
class AudioLoading : public BaseScene {
   private:
   public:
	std::shared_ptr<audio::WAVE> sound;
	AudioLoading() {
		sound = Man::Manager::Insatance->LoadOGG("test/effect.ogg");
	}
	~AudioLoading() {
	}

	void Update(int64_t){};
	void Render();
	void RenderGUI() {
		ImGui::Begin("effect");
		if (ImGui::Button("enqueue")) audio::audioOut->EnqueueSound(sound);
		ImGui::End();
	};
};

void AudioLoading::Render() {
	renderer->Drawn();
}

}  // namespace scene

int main() {
	Start("DuTest", 1);
}

void LoadAudio() {
	auto &korobe = Man::Manager::Insatance->LoadOGG("test/music.ogg");
	auto &music = audio::audioOut->EnqueueMusic(korobe);
	(void)music;
}

void Setup() {
	LoadAudio();
	new scene::AudioLoading;
}