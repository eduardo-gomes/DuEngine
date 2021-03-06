#include <DuEngine/DuEngine.hpp>
#include <cstdint>

namespace scene {
class MultiplesTextures : public BaseScene {
   private:
	Texture* tex[32];

   public:
	MultiplesTextures() {
		TextureParameters param(GL_LINEAR, GL_LINEAR);
		for (int i = 0; i < 32; ++i)
			tex[i] = new Texture("test/textures/" + std::to_string(i) + ".bmp", param);
	}
	~MultiplesTextures() {
		for (int i = 0; i < 32; ++i)
			delete tex[i];
	}

	void Update(int64_t){};
	void Render();
	void RenderGUI() {
		renderer->DispInfo();
	};
};

void MultiplesTextures::Render() {
	for (int x = 0; x < 8; ++x)
		for (int y = 0; y < 4; ++y) {
			vec3f position{((float)x / 4 - 1.0f) * 1.4f, (-(float)y / 1.75f + 1.0f), 0.0f};
			vec4f color{1.0f, 1.0f, 1.0f, 1.0f};
			vec2f size{0.3f, 0.3f};
			renderer->DrawnQuad(position, color, size, *tex[x + y * 8]);
		}
	renderer->Drawn();
}

}  // namespace scene

#include <iostream>
int main() {
	//audio::musicReserve(1);
	SetSetup([]() { new scene::MultiplesTextures; });
	Start("DuTest");
	return 0;
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
