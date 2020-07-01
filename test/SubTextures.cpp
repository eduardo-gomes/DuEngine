#include <cstdint>
#include <DuEngine/DuEngine.hpp>

namespace scene {
class SubTextures : public BaseScene {
   private:
	std::shared_ptr<Texture> tex;
	SubTexture* subtex[16 * 16];

   public:
	SubTextures() {
		keyboard::Repeat = 1;
		TextureParameters param(GL_LINEAR, GL_LINEAR);
		tex = std::make_shared<Texture>("test/textures/sheet.bmp", param);
		for (int x = 0; x < 16; ++x)
			for (int y = 0; y < 16; ++y)
				subtex[x + y * 16] = new SubTexture(tex, x, y, 16, 16);
	}
	~SubTextures() {}

	void Update(int64_t);
	void Render();
	void RenderGUI() {
		renderer->DispInfo();
	};
};
void SubTextures::Update(int64_t delta){
	static float x = 0.0f, y = 0.0f, z = 2.95f;
		if (keyboard::w) y += 0.2f * (delta / 10e6);
		if (keyboard::s) y -= 0.2f * (delta / 10e6);
		if (keyboard::d) x += 0.2f * (delta / 10e6);
		if (keyboard::a) x -= 0.2f * (delta / 10e6);
		if (keyboard::DOWN) z += 0.2f * (delta / 10e6);
		if (keyboard::UP) z -= 0.2f * (delta / 10e6);

		renderer->LookAt(x, y, z, x, y, 0.0f, 0.0f, 1.0f, 0.0f);
}
void SubTextures::Render() {
	constexpr vec3f position{0.0f, 0.0f, 1.0f};
	constexpr vec4f color{1.0f, 1.0f, 1.0f, 1.0f};
	constexpr vec2f size{1.0f, 1.0f};
	static int frame = 0;
	frame++;
	renderer->DrawnQuadRotate(position, color, size, (frame % 360), *subtex[frame % 256]);
	for (int x = 0; x < 16; ++x)
		for (int y = 0; y < 16; ++y) {
			vec3f position{(float)x, (float)y , 0.0f};
			vec4f color{1.0f, 1.0f, 1.0f, 1.0f};
			vec2f size{0.75f, 0.75f};
			renderer->DrawnQuad(position, color, size, *subtex[x + y * 16]);
		}
	renderer->Drawn();
}

}  // namespace scene

#include <iostream>
int main() {
	//audio::musicReserve(1);
	Start("DuTest");
}

void Setup() {
	new scene::SubTextures;
}