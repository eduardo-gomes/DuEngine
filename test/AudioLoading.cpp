#include <DuEngine/DuEngine.hpp>
#include <chrono>
#include <future>

namespace scene {
class AudioLoading : public BaseScene {
   private:
   public:
	std::shared_ptr<audio::WAVE> sound;
	AudioLoading() {
		sound = Man::Manager::Instance->LoadOGG("test/effect.ogg");
		printf("See music.txt for copyright info\n");
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
void Setup();
int main() {
	SetSetup(Setup);
	Start("DuTest", 1);
	return 0;
}

void LoadAudio() {
	auto &korobe = Man::Manager::Instance->LoadOGG("test/music.ogg");
	auto &music = audio::audioOut->EnqueueMusic(korobe);
	(void)music;
}

void Setup() {
	LoadAudio();
	new scene::AudioLoading;
}
#ifdef _WIN32
int WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd) {
	(void)hInstance;
	(void)hPrevInstance;
	(void)lpCmdLine;
	(void)nShowCmd;
	return main();
}
#endif