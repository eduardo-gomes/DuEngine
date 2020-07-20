#include <DuEngine/DuEngine.hpp>
#include <chrono>
#include <future>

namespace scene {
class TextureTest : public BaseScene {
   private:
	Texture* tex;

   public:
	TextureTest() {
		tex = new Texture("test/textures/bloco.bmp");
	}
	~TextureTest() {
		delete tex;
	}

	void Update(int64_t){};
	void Render();
	void RenderGUI(){};
};

void TextureTest::Render() {
	static float x = 0.0;
	x += 0.01f;
	if (x >= 0.9) x = -x;
	{
		vec3f position{x, 0.0f, 0.0f};
		vec4f color{0.0f, 0.35f, 1.0f, 1.0f};
		vec2f size{1.0f, 1.0f};
		renderer->DrawnQuad(position, color, size);
	}
	{
		vec3f position{0.0f, 0.5f, 0.0f};
		vec4f color{0.0f, 1.0f, 1.0f, 1.0f};
		vec2f size{1.0f, 1.0f};
		renderer->DrawnQuad(position, color, size, *tex);
	}
	renderer->Drawn();
}

}  // namespace scene

int main() {
	//audio::musicReserve(1);
	SetSetup([]() { new scene::TextureTest; });
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
